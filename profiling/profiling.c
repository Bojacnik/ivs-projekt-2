#ifndef DEBUG
#define DEBUG
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "gmp.h"
#include "mpfr.h"

//TODO: change

//Functions declaration //
int *handleInput(size_t *N); //this function gets N pointer and returns array of numbers added manually by user
double join_sqrt(double left_part, double right_part); // calculating square root of left_part * right_part
double left_part(int N); //this function gets number of measurements and returns firth part of formula which is 1/(N-1)
double arithmetic_mean(int N, const int *array); // 1/N * from i to N SUM(Xi)
double right_part(int N, const int *array, double C); // (sum(xi^2) - N * C^2)

double left_part(int N) { //this function gets number of measurements and returns firth part of formula which is 1/(N-1)
    return (double)1 / (double)(N-1);     //returning result
}

//this function gets number of measurements and array of numbers, returns arithmetic mean
double arithmetic_mean(int N, const int *array) {
    double part = (double)1 / (double)(N);  //(1/N)

    double sum = 0; //sum of entire array[n]
    for(size_t n = 0; n < N; n++)
    {
        sum += array[n];  //adding elements from array to make the sum
    }

    double result = part * sum;    // result = (1/N) * sum
    return result;  // returning result
}

//this function gets number of measurements, array and arithmetic mean, returns value of internal function (sum xi^2 - N * C^2)
double right_part(int N, const int *array, double C)
{
    double result = 0;    //result of function
    for(size_t i = 0; i < N; i++)  {
        // sums all second powers of array items
        result += array[i] * array[i];
    }
    result -= N * C * C;
    return result;
}

//this functions multiplies left_part by right_part and then returns its square root
double join_sqrt(double left_part, double right_part) {
    return sqrt(left_part * right_part);
}

//this function assigns number of scanned numbers to the N and returns a pointer to an array of numbers scanned from STDIN
int *handleInput(size_t *N) {
    int *array = NULL; //pointer for the array where all the input numbers will be stored
    int scan_validity;  // for checking input correctness
    size_t tmp_n = 0; // temporary iterable which will be later assigned to the original function argument N
    // (this way it is faster than incrementing dereferencing in a loop)

    long long int value;  //help integer variable for getting numbers by user

    // main loop for handling input
    while((scan_validity = scanf("%lld", &value)) != EOF )
    {
        // check where there were any errors during getting input
        if(scan_validity != 1) {
            fprintf(stderr, "Invalid input!\n"); //error message
            return NULL;
        }
        tmp_n++;
        //array pointer is reallocated because of new element in array
        array = realloc(array, tmp_n * sizeof(int));

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
    int *array = handleInput(&N); //user adds manually his values of measurements

    //if input has failed
    if(array == NULL) {
            fprintf(stderr, "Failed while handling input!");
            return 1;
    }
    double A = left_part(N);                                // firth part of formula
    double C = arithmetic_mean(N, array);                   // getting value of arithmetic mean
    double B = right_part(N, array, C);                     // sum (1, N): (xi^2 - N*C^2)^2
    double final_result = join_sqrt(A, B); // join_sqrt sqrt(A * B)

#ifdef DEBUG
    printf("Prvni cast: %.5f\n", A);      //THIS IS FOR TESTS ONLY
    printf("Arithmetic mean is: %5f\n", C); //THIS IS FOR TEST ONLY
    printf("Internal function is: %5f\n", B);   //THIS IS FOR TEST ONLY
    printf("Result: %5f\n", final_result);  //THIS IS FOR TEST ONLY
#endif
    free(array); // free array of input numbers
    return 0;
}
