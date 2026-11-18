/**
 * @file operation_tests.c
 * @brief Unit tests for the math library (src/backend/operation.c).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/backend/operation.h"

static int failures = 0;
static int total = 0;

/** Checks that @p actual equals @p expected, then frees @p actual. */
static void check(const char *expr, char *actual, const char *expected) {
	total++;
	if (actual == NULL || strcmp(actual, expected) != 0) {
		failures++;
		fprintf(stderr, "FAIL: %s\n  expected: %s\n  actual:   %s\n", expr, expected,
		        actual ? actual : "(null)");
	}
	free(actual);
}

/** Like check(), but compares the result rounded to @p digits significant digits. */
static void check_rounded(const char *expr, char *actual, int digits, const char *expected) {
	char *rounded = op_format(actual, digits);
	free(actual);
	check(expr, rounded, expected);
}

#define CHECK(call, expected) check(#call, (call), (expected))
#define CHECK_ROUNDED(call, digits, expected) check_rounded(#call, (call), (digits), (expected))

static void test_add(void) {
	CHECK(op_add("0.1", "0.2"), "0.3");
	CHECK(op_add("1000", "0.1"), "1000.1");
	CHECK(op_add("1000000", "0.0001"), "1000000.0001");
	CHECK(op_add("-5", "5"), "0");
	CHECK(op_add("1e30", "1"), "1e+30");
}

static void test_sub(void) {
	CHECK(op_sub("0.1", "0.2"), "-0.1");
	CHECK(op_sub("1000", "0.1"), "999.9");
	CHECK(op_sub("1000000", "0.0001"), "999999.9999");
	CHECK(op_sub("0.0000002", "0.0000001"), "1e-07");
}

static void test_mul(void) {
	CHECK(op_mul("0", "2535235"), "0");
	CHECK(op_mul("24069834", "0"), "0");
	CHECK(op_mul("-3", "0"), "0");
	CHECK(op_mul("5", "4"), "20");
	CHECK(op_mul("900", "2"), "1800");
	CHECK(op_mul("500.1", "2"), "1000.2");
	CHECK(op_mul("1000.1", "1000.2"), "1000300.02");
	CHECK(op_mul("-2", "3"), "-6");
}

static void test_div(void) {
	CHECK(op_div("5", "5"), "1");
	CHECK(op_div("100", "20"), "5");
	CHECK(op_div("1", "8"), "0.125");
	CHECK(op_div("514", "3"), "171.333333333333333333333333333");
	CHECK(op_div("1", "0"), OP_ERROR);
	CHECK(op_div("0", "0"), OP_ERROR);
}

static void test_factorial(void) {
	CHECK(op_factorial("0"), "1");
	CHECK(op_factorial("1"), "1");
	CHECK(op_factorial("5"), "120");
	CHECK(op_factorial("10"), "3628800");
	CHECK(op_factorial("-1"), OP_ERROR);
	CHECK(op_factorial("2.5"), OP_ERROR);
	CHECK(op_factorial("1e100"), OP_ERROR);
}

static void test_pow(void) {
	CHECK(op_pow("10", "0"), "1");
	CHECK(op_pow("10", "2"), "100");
	CHECK(op_pow("10", "9"), "1000000000");
	CHECK(op_pow("2", "-1"), "0.5");
	CHECK(op_pow("-2", "3"), "-8");
	CHECK(op_pow("0", "-1"), OP_ERROR);
	CHECK(op_pow("-8", "0.5"), OP_ERROR);
}

static void test_root(void) {
	CHECK(op_root("9", "2"), "3");
	CHECK(op_root("-8", "3"), "-2");
	CHECK(op_root("4", "-2"), "0.5");
	CHECK(op_root("-4", "2"), OP_ERROR);
	CHECK(op_root("4", "0"), OP_ERROR);
	CHECK_ROUNDED(op_root("2", "2"), 6, "1.41421");
	CHECK_ROUNDED(op_root("5", "3"), 6, "1.70998");
	CHECK_ROUNDED(op_root("2", "0.5"), 6, "4");
}

static void test_sin(void) {
	CHECK(op_sin("0"), "0");
	CHECK_ROUNDED(op_sin("2"), 6, "0.909297");
	CHECK_ROUNDED(op_sin("3.14"), 6, "0.00159265");
	CHECK_ROUNDED(op_sin("-2"), 6, "-0.909297");
}

static void test_invalid_input(void) {
	CHECK(op_add("abc", "1"), OP_ERROR);
	CHECK(op_add("1", ""), OP_ERROR);
	CHECK(op_add("1", NULL), OP_ERROR);
	CHECK(op_sin(OP_ERROR), OP_ERROR);
	CHECK(op_format("12abc", 10), OP_ERROR);
}

static void test_format(void) {
	CHECK(op_format("171.333333333333333333", 10), "171.3333333");
	CHECK(op_format("0.0000001", 10), "1e-07");
	CHECK(op_format("123456789012345", 10), "1.23456789e+14");
	CHECK(op_format("-0", 10), "0");
	CHECK(op_format("5.", 10), "5");
}

int main(void) {
	test_add();
	test_sub();
	test_mul();
	test_div();
	test_factorial();
	test_pow();
	test_root();
	test_sin();
	test_invalid_input();
	test_format();

	if (failures > 0) {
		fprintf(stderr, "%d of %d tests failed\n", failures, total);
		return EXIT_FAILURE;
	}
	printf("All %d tests passed\n", total);
	return EXIT_SUCCESS;
}
