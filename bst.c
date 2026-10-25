#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

struct btreenode    
{
    struct btreenode *left;
    int data;
    struct btreenode *right;
};
