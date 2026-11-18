/**
 * @file operation.h
 * @brief Arbitrary-precision math library used by the calculator.
 *
 * Every function takes its operands as decimal strings and returns a newly
 * allocated string that the caller must free(). On invalid input or an
 * undefined result (division by zero, even root of a negative number, ...)
 * the returned string is OP_ERROR.
 */
#pragma once

#include <stdbool.h>

/** Value returned (as a new string) when an operation fails. */
#define OP_ERROR "Err"

/** Number of significant digits in strings returned by the op_* functions. */
#define OP_RESULT_DIGITS 30

/** Unit of the argument of trigonometric functions. */
typedef enum {
	OP_RADIANS,
	OP_DEGREES,
} OpAngleUnit;

char *op_add(const char *addend1, const char *addend2);
char *op_sub(const char *minuend, const char *subtrahend);
char *op_mul(const char *multiplicand, const char *multiplier);
char *op_div(const char *dividend, const char *divisor);
char *op_pow(const char *base, const char *exponent);
char *op_root(const char *radicand, const char *index);
char *op_factorial(const char *n);
char *op_sin(const char *angle, OpAngleUnit unit);
char *op_cos(const char *angle, OpAngleUnit unit);
char *op_tan(const char *angle, OpAngleUnit unit);

/**
 * @brief Re-formats a number to at most @p digits significant digits.
 * @return New string, or OP_ERROR if @p number is not a valid number.
 */
char *op_format(const char *number, int digits);

/** @return true if @p result is the error value OP_ERROR. */
bool op_is_error(const char *result);
