//
// Created by bojacnik on 5.4.24.
//

#include "operation.h"
#include "string.h"
#include <assert.h>

void test_add_1() {
    //setup
    char *number1 = "150";
    char *number2 = "150";
    char *expected = "300";

    assert(strcmp(op_add(number1, number2), expected));
}

