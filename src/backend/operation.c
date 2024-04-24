#include <mpfr.h>
#include <math.h>
#include <malloc.h>
#include <string.h>
#include "stdlib.h"

#define MAX_PRECISION_DEC 30

#define PRECISION_DECIMALS 100
#define PRECISION_BITS ceil(PRECISION_DECIMALS * log2(10))
#define ROUNDING MPFR_RNDZ

/**
 * @brief removes substr from str
 * @param str string to remove from
 * @param sub substring to remove
 * @return string without the substring
 */
void remove_substring(char *str, const char *sub) {
    // Find the position of the substring in the string
    char *found = strstr(str, sub);
    if (found != NULL) {
        // Calculate the length of the substring
        size_t sub_length = strlen(sub);

        // Calculate the length of the remaining string after removing the substring
        size_t remaining_length = strlen(found + sub_length);

        // Shift the characters after the substring to cover it up
        memmove(found, found + sub_length, remaining_length + 1);

        // Adjust the length of the string
        str = realloc(str, remaining_length + 1);
    }
}

/**
 * @brief removes zeros at the end of the string
 * @param str string to remove zeros
 * @return str without zeros at the end
 */
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
/**
 * @brief converts mpfr number to a string
 * @param number to convert
 * @return number in form of string
 */
char *convertToString(mpfr_t number) {
    char *str = malloc(sizeof(char)*(102));
    if (str == NULL) {
        fprintf(stderr, "Error while allocating memory! Not Enough room on heap!");
        return NULL;
    }
    mpfr_snprintf(str, 100, "%.50Rf", number);
    removeTrailingZeros(str);
    if(strlen(str) >= 10){
        mpfr_snprintf(str, 100, "%.5Re", number);
    } else{
        mpfr_snprintf(str, 100, "%.5Rf", number);
        removeTrailingZeros(str);
    }

    remove_substring(str, "e+00");

    return str;
}


/**
 *
 *
 * @param addend1 First number string for addition
 * @param addend2 Second number string for addition
 * @return Resulting number as string
 */
char *op_add(char *addend1, char *addend2) {
    mpfr_t _addend1, _addend2;
    mpfr_init2(_addend1, PRECISION_BITS);
    mpfr_init2(_addend2, PRECISION_BITS);

    mpfr_init_set_str(_addend1, addend1, 10, ROUNDING);
    mpfr_init_set_str(_addend2, addend2, 10, ROUNDING);

    mpfr_t summand;
    mpfr_init2(summand, PRECISION_BITS);
    mpfr_add(summand, _addend1, _addend2, ROUNDING);

    char *result = convertToString(summand);
    mpfr_clears(_addend1, _addend2, summand, NULL);
    return result;
}

/**
 *
 * @brief subtraction for two numbers
 * @param minuend Number that is going to be subtracted from
 * @param subtrahend Number that will be used to subtract from minuend
 * @return Product of the minuend and subtrahend
 */
char *op_sub(char *minuend, char *subtrahend) {
    mpfr_t _minuend, _subtrahend;
    mpfr_init2(_minuend, PRECISION_BITS);
    mpfr_init2(_subtrahend, PRECISION_BITS);

    mpfr_init_set_str(_minuend, minuend, 10, ROUNDING);
    mpfr_init_set_str(_subtrahend, subtrahend, 10, ROUNDING);

    mpfr_t product;
    mpfr_init2(product, PRECISION_BITS);
    mpfr_sub(product, _minuend, _subtrahend, ROUNDING);

    char *result = convertToString(product);
    mpfr_clears(_minuend, _subtrahend, product, NULL);
    return result;
}

/**
 *
 * @brief multiplication for two numbers
 * @param multiplicand Number that will be multiplied by the multiplier
 * @param multiplier Number that will multiply the multiplicand
 * @return Product of the multiplicand and multiplier
 */
char *op_mul(char *multiplicand, char *multiplier) {
    mpfr_t _multiplicand, _multiplier;
    mpfr_init2(_multiplicand, PRECISION_BITS);
    mpfr_init2(_multiplier, PRECISION_BITS);

    mpfr_init_set_str(_multiplicand, multiplicand, 10, ROUNDING);
    mpfr_init_set_str(_multiplier, multiplier, 10, ROUNDING);

    mpfr_t product;
    mpfr_init2(product, PRECISION_BITS);
    mpfr_mul(product, _multiplicand, _multiplier, ROUNDING);

    char *result = convertToString(product);
    mpfr_clears(_multiplicand, _multiplier, product, NULL);
    return result;
}

/**
 *
 * \brief division for two numbers
 * \param dividend Number that will be divided by the divisor
 * \param divisor Number that will divide dividend
 * \return Quotient as string
 */
char *op_div(char *dividend, char *divisor) {
    mpfr_t _dividend, _divisor;
    mpfr_init2(_dividend, PRECISION_BITS);
    mpfr_init2(_divisor, PRECISION_BITS);

    mpfr_init_set_str(_dividend, dividend, 10, ROUNDING);
    mpfr_init_set_str(_divisor, divisor, 10, ROUNDING);

    mpfr_t quotient;
    mpfr_init2(quotient, PRECISION_BITS);
    mpfr_div(quotient, _dividend, _divisor, ROUNDING);

    char *str = convertToString(quotient);
    mpfr_clears(_dividend, _divisor, quotient, NULL);
    return str;
}

/**
 *
 * @brief math factorial
 * @param factor will determine to where we shall multiply numbers for example factor of five will result in 120
 * @return resulting factorial of factor
 */
char *op_factorial(char *factor) {

    if(factor[0] == '-'){
        char* str = malloc(102);
        strcpy(str, "Err");
        return str;
    }
    mpfr_t num1, num2, fac;
    mpfr_init2(num1, PRECISION_BITS);
    mpfr_init2(num2, PRECISION_BITS);
    mpfr_init2(fac, PRECISION_BITS);

    mpfr_init_set_str(fac, factor, 10, ROUNDING);
    mpfr_set_d(num2, 1, ROUNDING);
    mpfr_set_d(num1, 1, ROUNDING);


    for(; mpfr_cmp(num1, fac) <= 0; mpfr_add_d(num1, num1, 1, ROUNDING)) {
        mpfr_mul(num2, num1, num2, ROUNDING);
    }

    char* str = convertToString(num2);

    mpfr_clears(num1, num2, fac, NULL);
    return str;

}

/**
 *
 * @brief exponent power of base
 * @param base will be exponentiated by exponent
 * @param exponent will be used to exponentiate base
 * @return the exponentiated base by exponent
 */
char *op_pow(char* base, char* exponent) {
    mpfr_t _base, _exponent;
    mpfr_init2(_base, PRECISION_BITS);
    mpfr_init2(_exponent, PRECISION_BITS);

    mpfr_init_set_str(_base, base, 10, ROUNDING);
    mpfr_init_set_str(_exponent, exponent, 10, ROUNDING);

    mpfr_pow(_base, _base, _exponent, ROUNDING);
    char* str = convertToString(_base);
    return str;
}

/**
 *
 * @brief general root
 * @param radicand will be exponentiated by index
 * @param index will be used to exponentiate base
 * @return index-th root of radicant
 */
char *op_root(char *radicand, char *index) {
    mpfr_t _radicand, num2, root, one_over;

    mpfr_init_set_str(_radicand, radicand, 10, ROUNDING);
    mpfr_init_set_str(num2, index, 10, ROUNDING);

    mpfr_init2(root, PRECISION_BITS);
    mpfr_init2(one_over, PRECISION_BITS);

    mpfr_set_d(one_over, 1, ROUNDING);
    mpfr_div(one_over, one_over, num2, ROUNDING);

    mpfr_pow(root, _radicand, one_over, ROUNDING);

    char *result = convertToString(root);
    mpfr_clears(_radicand, num2, one_over, NULL);
    return result;
}


/**
 *
 * @brief general root
 * @param argument variable in radians used for sin
 * @return number from sin(argument)
 */
char *op_sin(char *argument) {
    mpfr_t _argument;
    mpfr_init2(_argument, PRECISION_BITS);

    mpfr_init_set_str(_argument, argument, 10, ROUNDING);

    mpfr_sin(_argument, _argument, ROUNDING);

    char *result = convertToString(_argument);
    mpfr_clear(_argument);
    return result;
}
