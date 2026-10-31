#include <stdio.h>
#define SIZE 10

int linear[SIZE],quadratic[SIZE],doubleHash[SIZE];

void initialize(){
    for (size_t i = 0; i < SIZE; i++)
    {
        linear[i] = quadratic[i] = doubleHash[i] = -1; 
    }
    
}
void Display( int table[]){

    printf("\n INDEX : KEY \n");
    for (size_t i = 0; i < SIZE; i++)
    {
        if (table[i] == -1)
        {
            printf(" %d -> EMPTY \n ",i);
        }
        printf(" %d -> %d \n ",i,table[i]);
    }
    

}

