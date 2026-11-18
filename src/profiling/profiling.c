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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../backend/operation.h"

/** Longest accepted input token. */
#define TOKEN_MAX 128

/**
 * @brief Replaces *acc with @p value, freeing the previous value.
 * @return false if @p value is NULL (out of memory).
 */
static bool update(char **acc, char *value) {
	free(*acc);
	*acc = value;
	return value != NULL;
}

typedef enum { READ_OK, READ_INVALID, READ_NO_MEMORY } ReadStatus;

/**
 * @brief Reads whitespace-separated numbers from stdin and accumulates
 *        their count, sum and sum of squares.
 */
static ReadStatus read_input(size_t *count, char **sum, char **sum_sq) {
	char token[TOKEN_MAX + 1];
	char format[16];
	snprintf(format, sizeof format, "%%%ds", TOKEN_MAX);

	while (scanf(format, token) == 1) {
		char *square = op_mul(token, token);
		if (square == NULL) {
			return READ_NO_MEMORY;
		}
		if (op_is_error(square)) {
			fprintf(stderr, "Invalid input: %s\n", token);
			free(square);
			return READ_INVALID;
		}
		bool ok = update(sum, op_add(*sum, token)) && update(sum_sq, op_add(*sum_sq, square));
		free(square);
		if (!ok) {
			return READ_NO_MEMORY;
		}
		(*count)++;
	}
	return READ_OK;
}

/**
 * @brief Sample standard deviation from the accumulated values.
 * @return New string with the result (or OP_ERROR), NULL if out of memory.
 */
static char *standard_deviation(size_t count, const char *sum, const char *sum_sq) {
	char n[32];
	snprintf(n, sizeof n, "%zu", count);

	char *mean = op_div(sum, n);
	char *n_minus_one = op_sub(n, "1");
	char *tmp = NULL;

	bool ok = mean != NULL && n_minus_one != NULL && update(&tmp, op_mul(mean, mean)) && // mean^2
	          update(&tmp, op_mul(n, tmp)) &&                                            // N * mean^2
	          update(&tmp, op_sub(sum_sq, tmp)) &&                                       // sum(x^2) - N * mean^2
	          update(&tmp, op_div(tmp, n_minus_one)) &&                                  // variance
	          update(&tmp, op_root(tmp, "2"));                                           // standard deviation

	free(mean);
	free(n_minus_one);
	if (!ok) {
		free(tmp);
		return NULL;
	}
	return tmp;
}

int main(void) {
	size_t count = 0;
	char *sum = op_add("0", "0");
	char *sum_sq = op_add("0", "0");
	int status = EXIT_FAILURE;

	ReadStatus read = (sum != NULL && sum_sq != NULL) ? read_input(&count, &sum, &sum_sq) : READ_NO_MEMORY;
	if (read == READ_NO_MEMORY) {
		fprintf(stderr, "Out of memory\n");
	} else if (read == READ_OK) {
		if (count < 2) {
			fprintf(stderr, "At least two numbers are required\n");
		} else {
			char *result = standard_deviation(count, sum, sum_sq);
			if (result == NULL) {
				fprintf(stderr, "Out of memory\n");
			} else if (op_is_error(result)) {
				fprintf(stderr, "Calculation failed\n");
			} else {
				printf("%s\n", result);
				status = EXIT_SUCCESS;
			}
			free(result);
		}
	}

	free(sum);
	free(sum_sq);
	return status;
}
