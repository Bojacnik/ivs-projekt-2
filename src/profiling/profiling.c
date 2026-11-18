/**
 * @file profiling.c
 * @brief Sample standard deviation of numbers read from stdin.
 *
 * Used to profile the math library. Computes
 *
 *     s = sqrt( (sum(x_i^2) - N * mean^2) / (N - 1) )
 *
 * where mean = sum(x_i) / N, using only the op_* functions.
 */
#include <stdio.h>
#include <stdlib.h>

#include "../backend/operation.h"

/** Longest accepted input token. */
#define TOKEN_MAX 128

/** Replaces *acc with @p value, freeing the previous value. */
static void update(char **acc, char *value) {
	free(*acc);
	*acc = value;
}

/**
 * @brief Reads whitespace-separated numbers from stdin and accumulates
 *        their count, sum and sum of squares.
 * @return 0 on success, -1 on invalid input.
 */
static int read_input(size_t *count, char **sum, char **sum_sq) {
	char token[TOKEN_MAX + 1];
	char format[16];
	snprintf(format, sizeof format, "%%%ds", TOKEN_MAX);

	while (scanf(format, token) == 1) {
		char *square = op_mul(token, token);
		if (op_is_error(square)) {
			fprintf(stderr, "Invalid input: %s\n", token);
			free(square);
			return -1;
		}
		update(sum, op_add(*sum, token));
		update(sum_sq, op_add(*sum_sq, square));
		free(square);
		(*count)++;
	}
	return 0;
}

/**
 * @brief Sample standard deviation from the accumulated values.
 * @return New string with the result, or OP_ERROR.
 */
static char *standard_deviation(size_t count, const char *sum, const char *sum_sq) {
	char n[32];
	snprintf(n, sizeof n, "%zu", count);

	char *mean = op_div(sum, n);

	char *tmp = op_mul(mean, mean);       // mean^2
	update(&tmp, op_mul(n, tmp));         // N * mean^2
	update(&tmp, op_sub(sum_sq, tmp));    // sum(x^2) - N * mean^2

	char *n_minus_one = op_sub(n, "1");
	update(&tmp, op_div(tmp, n_minus_one)); // variance
	update(&tmp, op_root(tmp, "2"));        // standard deviation

	free(n_minus_one);
	free(mean);
	return tmp;
}

int main(void) {
	size_t count = 0;
	char *sum = op_add("0", "0");
	char *sum_sq = op_add("0", "0");

	int status = EXIT_FAILURE;
	if (read_input(&count, &sum, &sum_sq) == 0) {
		if (count < 2) {
			fprintf(stderr, "At least two numbers are required\n");
		} else {
			char *result = standard_deviation(count, sum, sum_sq);
			if (!op_is_error(result)) {
				printf("%s\n", result);
				status = EXIT_SUCCESS;
			} else {
				fprintf(stderr, "Calculation failed\n");
			}
			free(result);
		}
	}

	free(sum);
	free(sum_sq);
	return status;
}
