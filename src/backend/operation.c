/**
 * @file operation.c
 * @brief Implementation of the calculator math library on top of MPFR.
 */
#include "operation.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpfr.h>

/** Working precision: ~100 decimal digits. */
#define PRECISION_BITS 333
#define ROUNDING MPFR_RNDN

/** Largest n accepted by op_factorial. */
#define FACTORIAL_MAX 10000UL

typedef int (*binary_fn)(mpfr_ptr, mpfr_srcptr, mpfr_srcptr, mpfr_rnd_t);
typedef int (*unary_fn)(mpfr_ptr, mpfr_srcptr, mpfr_rnd_t);

static char *copy_string(const char *str) {
	char *copy = malloc(strlen(str) + 1);
	if (copy == NULL) {
		fprintf(stderr, "Out of memory\n");
		return NULL;
	}
	return strcpy(copy, str);
}

static char *error_result(void) {
	return copy_string(OP_ERROR);
}

/**
 * @brief Parses a decimal string into an already initialised number.
 * @return true if the whole string is a valid number.
 */
static bool parse(mpfr_t rop, const char *str) {
	return str != NULL && mpfr_set_str(rop, str, 10, ROUNDING) == 0;
}

/**
 * @brief Formats a number with at most @p digits significant digits.
 *
 * Trailing zeros are dropped and very large or small values use scientific
 * notation. Infinity and NaN are reported as OP_ERROR.
 */
static char *to_string(mpfr_t number, int digits) {
	if (!mpfr_number_p(number)) {
		return error_result();
	}
	if (mpfr_zero_p(number)) {
		return copy_string("0"); // avoid printing "-0"
	}

	char *formatted = NULL;
	if (mpfr_asprintf(&formatted, "%.*Rg", digits, number) < 0) {
		return error_result();
	}
	char *result = copy_string(formatted);
	mpfr_free_str(formatted);
	return result;
}

static char *binary_op(const char *a, const char *b, binary_fn fn) {
	mpfr_t x, y;
	mpfr_inits2(PRECISION_BITS, x, y, (mpfr_ptr) NULL);

	char *result;
	if (parse(x, a) && parse(y, b)) {
		fn(x, x, y, ROUNDING);
		result = to_string(x, OP_RESULT_DIGITS);
	} else {
		result = error_result();
	}

	mpfr_clears(x, y, (mpfr_ptr) NULL);
	return result;
}

static char *unary_op(const char *a, unary_fn fn) {
	mpfr_t x;
	mpfr_init2(x, PRECISION_BITS);

	char *result;
	if (parse(x, a)) {
		fn(x, x, ROUNDING);
		result = to_string(x, OP_RESULT_DIGITS);
	} else {
		result = error_result();
	}

	mpfr_clear(x);
	return result;
}

/**
 * @brief index-th root of x.
 *
 * Integer indexes use mpfr_rootn_ui so that odd roots of negative numbers
 * work (cube root of -8 is -2). Other indexes are computed as x^(1/index).
 */
static int root(mpfr_ptr rop, mpfr_srcptr x, mpfr_srcptr index, mpfr_rnd_t rnd) {
	if (mpfr_zero_p(index)) {
		mpfr_set_nan(rop);
		return 0;
	}
	if (mpfr_integer_p(index) && mpfr_cmpabs_ui(index, LONG_MAX) <= 0) {
		long n = mpfr_get_si(index, rnd);
		int ret = mpfr_rootn_ui(rop, x, (unsigned long) labs(n), rnd);
		return n > 0 ? ret : mpfr_ui_div(rop, 1, rop, rnd);
	}

	mpfr_t exponent;
	mpfr_init2(exponent, mpfr_get_prec(rop));
	mpfr_ui_div(exponent, 1, index, rnd);
	int ret = mpfr_pow(rop, x, exponent, rnd);
	mpfr_clear(exponent);
	return ret;
}

/**
 * @brief n! for integer 0 <= n <= FACTORIAL_MAX, NaN otherwise.
 */
static int factorial(mpfr_ptr rop, mpfr_srcptr n, mpfr_rnd_t rnd) {
	if (!mpfr_integer_p(n) || mpfr_sgn(n) < 0 || mpfr_cmp_ui(n, FACTORIAL_MAX) > 0) {
		mpfr_set_nan(rop);
		return 0;
	}
	return mpfr_fac_ui(rop, mpfr_get_ui(n, rnd), rnd);
}

/** @brief Sum of two numbers. */
char *op_add(const char *addend1, const char *addend2) {
	return binary_op(addend1, addend2, mpfr_add);
}

/** @brief Difference minuend - subtrahend. */
char *op_sub(const char *minuend, const char *subtrahend) {
	return binary_op(minuend, subtrahend, mpfr_sub);
}

/** @brief Product of two numbers. */
char *op_mul(const char *multiplicand, const char *multiplier) {
	return binary_op(multiplicand, multiplier, mpfr_mul);
}

/** @brief Quotient dividend / divisor; OP_ERROR when dividing by zero. */
char *op_div(const char *dividend, const char *divisor) {
	return binary_op(dividend, divisor, mpfr_div);
}

/** @brief base raised to exponent. */
char *op_pow(const char *base, const char *exponent) {
	return binary_op(base, exponent, mpfr_pow);
}

/** @brief index-th root of radicand; OP_ERROR for even roots of negatives. */
char *op_root(const char *radicand, const char *index) {
	return binary_op(radicand, index, root);
}

/** @brief n!; OP_ERROR unless n is an integer in [0, 10000]. */
char *op_factorial(const char *n) {
	return unary_op(n, factorial);
}

/** @brief Sine of an angle in radians. */
char *op_sin(const char *argument) {
	return unary_op(argument, mpfr_sin);
}

char *op_format(const char *number, int digits) {
	mpfr_t x;
	mpfr_init2(x, PRECISION_BITS);
	char *result = parse(x, number) ? to_string(x, digits) : error_result();
	mpfr_clear(x);
	return result;
}

bool op_is_error(const char *result) {
	return result == NULL || strcmp(result, OP_ERROR) == 0;
}
