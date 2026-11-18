/**
 * @file test_util.h
 * @brief Minimal test helpers shared by the test programs.
 */
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int test_failures = 0;
static int test_total = 0;

/** Records a failed check unless @p actual equals @p expected. */
static inline void check_str(const char *what, const char *actual, const char *expected) {
	test_total++;
	if (actual == NULL || strcmp(actual, expected) != 0) {
		test_failures++;
		fprintf(stderr, "FAIL: %s\n  expected: \"%s\"\n  actual:   \"%s\"\n", what, expected,
		        actual ? actual : "(null)");
	}
}

/** Records a failed check unless @p condition holds. */
static inline void check_true(const char *what, int condition) {
	test_total++;
	if (!condition) {
		test_failures++;
		fprintf(stderr, "FAIL: %s\n", what);
	}
}

/** Prints the summary; returns the process exit code. */
static inline int test_report(const char *suite) {
	if (test_failures > 0) {
		fprintf(stderr, "%s: %d of %d checks failed\n", suite, test_failures, test_total);
		return EXIT_FAILURE;
	}
	printf("%s: all %d checks passed\n", suite, test_total);
	return EXIT_SUCCESS;
}
