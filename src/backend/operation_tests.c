//
// Created by bojacnik on 5.4.24.
//

#include "operation.h"
#include "string.h"
#include <assert.h>
#include <stdio.h>

void test_addition(char *number1, char *number2, char* expected) {
    assert(!strcmp(op_add(number1, number2), expected));
}

void test_subtraction(char *number1, char *number2, char* expected) {
    assert(!strcmp(op_sub(number1, number2), expected));
}

void test_multiplication(char *number1, char* number2, char* expected) {
    assert(!strcmp(op_mul(number1, number2), expected));
}

void test_division(char *number1, char* divisor, char* expected) {
    assert(!strcmp(op_div(number1, divisor), expected));
}

void test_factorial(char *number, char *expected) {
    assert(!strcmp(op_factorial(number), expected));
}

void test_power(char *base, char *exp, char *expected) {
    assert(!strcmp(op_pow(base, exp), expected));
}

void test_root(char *number, char* exponent, char* expected) {
    assert(!strcmp(op_root(number, exponent), expected));
}

void test_sin(char *number, char* expected) {
    assert(!strcmp(op_sin(number), expected));
}

int main(void) {
    test_addition("0.1", "0.2", "3.00000e-01");
    test_addition("1000", "0.1", "1.00010e+03");
    test_addition("1000000", "0.0001", "1.00000e+06");

    test_subtraction("0.1", "0.2", "-1.00000e-01");
    test_subtraction("1000", "0.1", "9.99900e+02");
    test_subtraction("1000000", "0.0001", "1.00000e+06");

    test_multiplication("0", "2535235", "0");
    test_multiplication("24069834", "0", "0");
    test_multiplication("5", "4", "20");
    test_multiplication("4", "5", "20");
    test_multiplication("900", "2", "1800");
    test_multiplication("2", "900", "1800");
    test_multiplication("500.1", "2", "1.00020e+03");
    test_multiplication("1000.1", "1000.2", "1.00030e+06");

    test_division("5", "5", "1");
    test_division("100", "20", "5");
    test_division("514", "3", "1.71333e+02");

    test_factorial("0", "1");
    test_factorial("1", "1");
    test_factorial("5", "120");
    test_factorial("10", "3628800");

    test_power("10", "0", "1");
    test_power("10", "1", "10");
    test_power("10", "2", "100");
    test_power("10", "9", "1.00000e+09");

    test_root("2", "2", "1.41421");
    test_root("5", "3", "1.70998");

    test_sin("2", "9.09297e-01");
    test_sin("3.14", "1.59265e-03");
    fprintf(stdout, "Tests successfull!");
}