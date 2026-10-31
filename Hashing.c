#include <stdio.h>
#define SIZE 10

int linear[SIZE],quadratic[SIZE],doubleHash[SIZE];

void initialize(){
    for (size_t i = 0; i < SIZE; i++)
    {
        linear[i] = quadratic[i] = doubleHash[i] = -1; 
    }
    
}
