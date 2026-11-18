/**
 * @file alloc_failure_tests.c
 * @brief Checks that failed memory allocations are handled, not dereferenced.
 *
 * Linked with -Wl,--wrap=malloc: every malloc() in our code goes through
 * __wrap_malloc(), which fails the N-th call. Each scenario is run with
 * N = 1, 2, 3, ... until it no longer allocates. MPFR/GMP's own allocations
 * are not wrapped (GMP aborts on allocation failure by design).
 */
#include <stddef.h>

#include "../src/backend/operation.h"
#include "../src/calculator/calculator.h"
#include "test_util.h"

void *__real_malloc(size_t size);

/** Number of mallocs left before one fails; negative = never fail. */
static int fail_countdown = -1;
static int failures_injected = 0;

void *__wrap_malloc(size_t size) {
	if (fail_countdown == 0) {
		fail_countdown = -1;
		failures_injected++;
		return NULL;
	}
	if (fail_countdown > 0) {
		fail_countdown--;
	}
	return __real_malloc(size);
}

/** Result must be NULL (out of memory) or the correct value. */
static void check_result(const char *what, int fail_at, char *actual, const char *expected) {
	char label[128];
	snprintf(label, sizeof label, "%s with malloc #%d failing", what, fail_at + 1);
	check_true(label, actual == NULL || strcmp(actual, expected) == 0);
	check_true(label, actual != NULL || op_is_error(actual));
	free(actual);
}

/** Runs @p call with each of its allocations failing in turn. */
#define FOR_EACH_FAILING_MALLOC(call, expected)                                                                        \
	for (int fail_at = 0;; fail_at++) {                                                                                \
		int before = failures_injected;                                                                                \
		fail_countdown = fail_at;                                                                                      \
		check_result(#call, fail_at, (call), (expected));                                                              \
		fail_countdown = -1;                                                                                           \
		if (failures_injected == before) {                                                                             \
			break; /* ran without reaching the failing malloc: all covered */                                          \
		}                                                                                                              \
	}

static void test_operations(void) {
	FOR_EACH_FAILING_MALLOC(op_add("1", "2"), "3");
	FOR_EACH_FAILING_MALLOC(op_div("1", "0"), OP_ERROR);
	FOR_EACH_FAILING_MALLOC(op_mul("0", "5"), "0");
	FOR_EACH_FAILING_MALLOC(op_root("-8", "3"), "-2");
	FOR_EACH_FAILING_MALLOC(op_factorial("5"), "120");
	FOR_EACH_FAILING_MALLOC(op_sin("30", OP_DEGREES), "0.5");
	FOR_EACH_FAILING_MALLOC(op_format("abc", 10), OP_ERROR);
}

/** The calculator must show the right result or Err, never crash. */
static void test_calculator(void) {
	const char *keys = "12+3=s4n*5!=";
	for (int fail_at = 0;; fail_at++) {
		int before = failures_injected;
		Calculator calc;
		calc_init(&calc);
		fail_countdown = fail_at;
		for (const char *k = keys; *k != '\0'; k++) {
			calc_handle(&calc, calc_action_for_char(*k));
		}
		fail_countdown = -1;

		char label[64];
		snprintf(label, sizeof label, "calculator with malloc #%d failing", fail_at + 1);
		check_true(label, calc.error == op_is_error(calc.display));
		if (failures_injected == before) {
			check_str("calculator without failures", calc.display, "-480");
			break;
		}
	}

	for (int fail_at = 0;; fail_at++) {
		int before = failures_injected;
		Calculator calc;
		calc_init(&calc);
		calc_handle(&calc, CALC_DIGIT_7);
		fail_countdown = fail_at;
		bool pasted = calc_paste(&calc, "42");
		fail_countdown = -1;

		check_true("failed paste keeps display",
		           pasted ? strcmp(calc.display, "42") == 0 : strcmp(calc.display, "7") == 0);
		if (failures_injected == before) {
			check_true("paste without failures", pasted);
			break;
		}
	}
}

int main(void) {
	test_operations();
	test_calculator();
	check_true("some allocations were made to fail", failures_injected > 0);
	printf("(%d allocation failures injected)\n", failures_injected);
	return test_report("allocation failures");
}
