#include <stdio.h>
#define SIZE 10

int linear[SIZE], quadratic[SIZE], doubleHash[SIZE];

void initialize()
{
    for (size_t i = 0; i < SIZE; i++)
    {
        linear[i] = quadratic[i] = doubleHash[i] = -1;
    }
}
void Display(int table[])
{

    printf("\n INDEX : KEY \n");
    for (size_t i = 0; i < SIZE; i++)
    {
        if (table[i] == -1)
        {
            printf(" %d -> EMPTY \n ", i);
        }
        else
        {
            printf(" %d -> %d \n ", i, table[i]);
        }
    }
}
void insert(int table[], int key, int method)
{

    if (key == 0)
    {
        printf("Enter a Postive Key \n");
        return;
    }
    
    int h = key % SIZE;
    int step = 7 - key % 7;
    int pos;

    for (size_t i = 0; i < SIZE; i++)
    {
        if (method == 1)
        {
            pos = (h + i) % SIZE;
            
        }
        else if (method == 2)
        {
            pos = (h + i * i) % SIZE;
            
        }
        else{
             pos = ( h + i*step)%SIZE;
            
        }

        if ( table[pos] = -1)
        {
            table[pos] = key;
            printf(" key Inserted \n");
            return;
        }
        printf("NO SLOT AVAILBLE \n");
    }
}
