

#include <mpfr.h>
#include <math.h>
#include <malloc.h>

#define MAX_PRECISION_DEC 30
#define PRECISION_BITS ceil(log(MAX_PRECISION_DEC) * 1 / log(10))


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

    mpfr_init_set_str(num1, number1, 10, MPFR_RNDN);
    mpfr_init_set_str(num2, number2, 10, MPFR_RNDN);

    mpfr_t additionResult;
    mpfr_init2(additionResult, PRECISION_BITS);

    mpfr_add(additionResult, num1, num2, MPFR_RNDN);

    char *str;
    str = mpfr_get_str(NULL, NULL, 10, 0, additionResult, MPFR_RNDN);
    mpfr_clears(num1, num2, additionResult, NULL);

    return str;
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

    char *str;
    str = mpfr_get_str(NULL, NULL, 10, 0, additionResult, MPFR_RNDN);
    mpfr_clears(num1, num2, additionResult, NULL);

    return str;
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

    char *str;
    str = mpfr_get_str(NULL, NULL, 10, 0, additionResult, MPFR_RNDN);
    mpfr_clears(num1, num2, additionResult, NULL);

    return str;
}

/**
 *
 *
 * @param number1 First number string for addition
 * @param number2 Second number string for addition
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

    char *str;
    str = mpfr_get_str(NULL, NULL, 10, 0, additionResult, MPFR_RNDN);
    mpfr_clears(num1, num2, additionResult, NULL);

    return str;
}


int main() {

    char *str;
    str = op_add("1", "2");

    printf("result %s\n", str);

    return 0;
}


