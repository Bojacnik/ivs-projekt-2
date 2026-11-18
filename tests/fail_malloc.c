/**
 * @file fail_malloc.c
 * @brief Link with -Wl,--wrap=malloc to make the N-th malloc() fail,
 *        where N is taken from the FAIL_AT environment variable (0-based).
 */
#include <stdlib.h>

void *__real_malloc(size_t size);

void *__wrap_malloc(size_t size) {
	static int countdown = -2; // -2 = not read yet, -1 = never fail
	if (countdown == -2) {
		const char *env = getenv("FAIL_AT");
		countdown = env != NULL ? atoi(env) : -1;
	}
	if (countdown == 0) {
		countdown = -1;
		return NULL;
	}
	if (countdown > 0) {
		countdown--;
	}
	return __real_malloc(size);
}
