#ifndef DEBUG
#define DEBUG
#endif

#define PRECISION_DECIMALS 100
#define PRECISION_BITS ceil(PRECISION_DECIMALS * log2(10))
#define rounding MPFR_RNDZ

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "gmp.h"
#include "mpfr.h"
#include "operation.h"

//TODO: change

//Functions declaration //
long double *handleInput(size_t *N); //this function gets N pointer and returns array of numbers added manually by user
char *join_sqrt(char *left_part, char *right_part); // calculating square root of left_part * right_part
char *left_part(const size_t *N); //this function gets number of measurements and returns firth part of formula which is 1/(N-1)
char *arithmetic_mean(size_t *N, const long double *array); // 1/N * from i to N SUM(Xi)
char *right_part(size_t *N, const long double *array, char *C); // (sum(xi^2) - N * C^2)

char* left_part(const size_t *N) { //this function gets number of measurements and returns firth part of formula which is 1/(N-1)
    char one[] = "1";

    char *n = malloc(sizeof(char)*22);
    if (n == NULL) {
        fprintf(stderr, "Heap allocation error");
        return NULL;
    }
    snprintf(n, 22, "%zul", *N);
    //returning result
    return op_div(one, op_sub(n, one));
}

char *convertUlToString(size_t *number) {
    char *n = malloc(sizeof(char)*22);
    if (n == NULL) {
        fprintf(stderr, "Heap allocation error");
        return NULL;
    }
    snprintf(n, 22, "%zul", *number);
    return n;
}

char *convertLongDoubleToString(const long double *number) {
    char *n = malloc(sizeof(char)*101);
    if (n == NULL) {
        fprintf(stderr, "Heap allocation error");
        return NULL;
    }
    snprintf(n, 101, "%Lf", *number);
    return n;
}

//this function gets number of measurements and array of numbers, returns arithmetic mean
char *arithmetic_mean(size_t *N, const long double *array) {
    char *part = op_div("1", convertUlToString(N));  //(1/N)

    char *sum = "0\0"; //sum of entire array[n]
    for(size_t n = 0; n < *N; n++)
    {
        sum = op_add(
                sum,
                convertLongDoubleToString(&(array[n]))
        );  //adding elements from array to make the sum
    }
    return op_mul(part, sum);  // (1/N) * sum
}

//this function gets number of measurements, array and arithmetic mean, returns value of internal function (sum xi^2 - N * C^2)
char *right_part(size_t *N, const long double *array, char *C)
{
    char *arraySum = "0\0";    //arraySum of function
    for(size_t i = 0; i < *N; i++)  {
        // sums all second powers of array items
        arraySum = op_add(
                arraySum,
                op_mul(
                        convertLongDoubleToString(&(array[i])),
                        convertLongDoubleToString(&(array[i]))
                )
        );
    }
    arraySum = op_sub(
            arraySum,
            op_mul(
                    convertUlToString(N),
                    op_mul(
                            C,
                            C)
                    )
    );

    return arraySum;
}

//this functions multiplies left_part by right_part and then returns its square root
char *join_sqrt(char *left_part, char *right_part) {
    return op_root(
            op_mul(
                    left_part,
                    right_part
                    ),
                    "2"
    );
}

//this function assigns number of scanned numbers to the N and returns a pointer to an array of numbers scanned from STDIN
long double *handleInput(size_t *N) {
    long double *array = NULL; //pointer for the array where all the input numbers will be stored
    int scan_validity;  // for checking input correctness
    size_t tmp_n = 0; // temporary iterable which will be later assigned to the original function argument N
    // (this way it is faster than incrementing dereferencing in a loop)

    long double value;  //help integer variable for getting numbers by user

    // main loop for handling input
    while((scan_validity = scanf("%Lf", &value)) != EOF )
    {
        // check where there were any errors during getting input
        if(scan_validity != 1) {
            fprintf(stderr, "Invalid input!\n"); //error message
            return NULL;
        }
        tmp_n++;
        //array pointer is reallocated because of new element in array
        array = realloc(array, tmp_n * sizeof(long double));

        //handle allocation failure
        if (array == NULL) {
            fprintf(stderr, "Heap allocation error!\n"); //error message
            return NULL;    //return NULL in error situation
        }
        array[tmp_n - 1] = value;  //adding value to the array
    }
    // assign the iterable to the original N
    (*N) = tmp_n;
    //return pointer to the array with numbers taken from the input
    return array;
}

/// IMPORTANT ///
// program to calculate the standard deviation is divided into individual parts,
// A = 1 / (N-1)
// B = sum (1, N): (xi^2 - N*C^2)^2
// C = arithmetic mean of measurements
// s= final result => s = sqrt(A*B)

int main(void) {
    size_t N = 0; //number of measurements
    long double *array = handleInput(&N); //user adds manually his values of measurements

    //if input has failed
    if(array == NULL) {
            fprintf(stderr, "Failed while handling input!");
            return 1;
    }
    char *A = left_part(&N);                                // firth part of formula
    char *C = arithmetic_mean(&N, array);                   // getting value of arithmetic mean
    char *B = right_part(&N, array, C);                     // sum (1, N): (xi^2 - N*C^2)^2
    char *final_result = join_sqrt(A, B); // join_sqrt sqrt(A * B)

#ifdef DEBUG
    printf("Prvni cast: %.5s\n", A);      //THIS IS FOR TESTS ONLY
    printf("Arithmetic mean is: %5s\n", C); //THIS IS FOR TEST ONLY
    printf("Internal function is: %5s\n", B);   //THIS IS FOR TEST ONLY
    printf("Result: %5s\n", final_result);  //THIS IS FOR TEST ONLY
#endif
    free(array); // free array of input numbers
    return 0;
}
