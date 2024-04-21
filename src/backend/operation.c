#include <mpfr.h>
#include <math.h>
#include <malloc.h>
#include <string.h>

#define MAX_PRECISION_DEC 30

#define PRECISION_DECIMALS 100
#define PRECISION_BITS ceil(PRECISION_DECIMALS * log2(10))

void removeTrailingZeros(char* str) {
    int len = strlen(str);
    for (int i = len - 1; i >= 0; i--) {
        if (str[i] == '0') {
            str[i] = '\0'; // Replace the zero with a null terminator
        } else if (str[i] == '.') {
            str[i] = '\0';
            break;
        } else {
            break;
        }
    }
}

char *convertToString(mpfr_t number) {
    char *str = malloc(sizeof(char)*(102));
    if (str == NULL) {
        fprintf(stderr, "Error while allocating memory! Not Enough room on heap!");
        return NULL;
    }
    mpfr_snprintf(str, 100, "%.5Rf", number);
    removeTrailingZeros(str);
    return str;
}


/**
 *
 *
 * @param number1 First number string for addition
 * @param number2 Second number string for addition
 * @return result Resulting number as string
 */
char *op_add(char *number1, char *number2) {
    mpfr_t num1, num2;
    mpfr_init2(num1, PRECISION_BITS);
    mpfr_init2(num2, PRECISION_BITS);

    mpfr_init_set_str(num1, number1, 10, MPFR_RNDA);
    mpfr_init_set_str(num2, number2, 10, MPFR_RNDA);

    mpfr_t additionResult;
    mpfr_init2(additionResult, PRECISION_BITS);

    mpfr_add(additionResult, num1, num2, MPFR_RNDA);

    char *result = convertToString(additionResult);
    mpfr_clears(num1, num2, additionResult, NULL);

    return result;
}

/**
 *
 *
 * @param number1 First number string for addition
 * @param number2 Second number string for addition
 * @return result Resulting number as string
 */
char *op_sub(char *number1, char *number2) {
    mpfr_t num1, num2;
    mpfr_init2(num1, PRECISION_BITS);
    mpfr_init2(num2, PRECISION_BITS);

    mpfr_init_set_str(num1, number1, 10, MPFR_RNDN);
    mpfr_init_set_str(num2, number2, 10, MPFR_RNDN);

    mpfr_t additionResult;
    mpfr_init2(additionResult, PRECISION_BITS);

    mpfr_sub(additionResult, num1, num2, MPFR_RNDN);

    char *result = convertToString(additionResult);

    mpfr_clears(num1, num2, additionResult, NULL);

    return result;
}

/**
 *
 *
 * @param number1 First number string for addition
 * @param number2 Second number string for addition
 * @return result Resulting number as string
 */
char *op_mul(char *number1, char *number2) {
    mpfr_t num1, num2;
    mpfr_init2(num1, PRECISION_BITS);
    mpfr_init2(num2, PRECISION_BITS);


    mpfr_init_set_str(num1, number1, 10, MPFR_RNDN);
    mpfr_init_set_str(num2, number2, 10, MPFR_RNDN);

    mpfr_t additionResult;
    mpfr_init2(additionResult, PRECISION_BITS);

    mpfr_mul(additionResult, num1, num2, MPFR_RNDN);

    char *result = convertToString(additionResult);
    mpfr_clears(num1, num2, additionResult, NULL);

    return result;
}

/**
 *
 *
 * @param number1 First number string for addition
 * @param number2 Second number string for divisor
 * @return result Resulting number as string
 */
char *op_div(char *number1, char *number2) {
    mpfr_t num1, num2;
    mpfr_init2(num1, PRECISION_BITS);
    mpfr_init2(num2, PRECISION_BITS);


    mpfr_init_set_str(num1, number1, 10, MPFR_RNDN);
    mpfr_init_set_str(num2, number2, 10, MPFR_RNDN);

    mpfr_t additionResult;
    mpfr_init2(additionResult, PRECISION_BITS);

    mpfr_div(additionResult, num1, num2, MPFR_RNDN);

    char *str = convertToString(num2);
    mpfr_clears(num1, num2, additionResult, NULL);

    return str;
}

char *op_factorial(char *factor) {

    if (!strcmp(factor, "0")) {
        char *zeroresult = malloc(sizeof(char)*2);
        if (zeroresult == NULL) {
            fprintf(stderr, "Failed to allocate memory on heap!");
            return NULL;
        }
        zeroresult[0] = '0';
        zeroresult[1] = '\0';
        return zeroresult;
    }

    mpfr_t num1, num2, fac;
    mpfr_init2(num1, PRECISION_BITS);
    mpfr_init2(num2, PRECISION_BITS);
    mpfr_init2(fac, PRECISION_BITS);

    mpfr_init_set_str(fac, factor, 10, MPFR_RNDN);
    mpfr_set_d(num2, 1, MPFR_RNDN);
    mpfr_set_d(num1, 1, MPFR_RNDN);


    for(; mpfr_cmp(num1, fac) <= 0; mpfr_add_d(num1, num1, 1, MPFR_RNDN)) {
        mpfr_mul(num2, num1, num2, MPFR_RNDN);
    }

    char *str = malloc(sizeof(char)*(102));
    if (str == NULL) {
        fprintf(stderr, "Error while allocating memory! Not Enough room on heap!");
        return NULL;
    }
    mpfr_snprintf(str, 100, "%.5Rf", num2);
    removeTrailingZeros(str);

    mpfr_clears(num1, num2, fac, NULL);

    return str;

}

char *op_pow(char* number, char* exp) {
    mpfr_t num1, num2;

    mpfr_init_set_str(num1, number, 10, MPFR_RNDN);
    mpfr_init_set_str(num2, exp, 10, MPFR_RNDN);

    mpfr_pow(num1, num1, num2, MPFR_RNDN);

    char* str = convertToString(num1);

    return str;
}

char *op_root(char *number, char *exponent) {
    mpfr_t num1, num2, res, one_over;

    mpfr_init_set_str(num1, number, 10, MPFR_RNDN);
    mpfr_init_set_str(num2, exponent, 10, MPFR_RNDN);

    mpfr_init2(res, PRECISION_BITS);
    mpfr_init2(one_over, PRECISION_BITS);

    mpfr_set_d(one_over, 1, MPFR_RNDN);
    mpfr_div(one_over, one_over, num2, MPFR_RNDN);

    mpfr_pow(res, num1, one_over, MPFR_RNDN);


    char *result = convertToString(res);

    mpfr_clears(num1, num2, one_over, NULL);

    return result;
}


//rads!
char *op_sin(char *number) {
    mpfr_t num1;
    mpfr_init_set_str(num1, number, 10, MPFR_RNDN);

    mpfr_sin(num1, num1, MPFR_RNDN);

    char *result = convertToString(num1);

    mpfr_clear(num1);

    return result;
}
