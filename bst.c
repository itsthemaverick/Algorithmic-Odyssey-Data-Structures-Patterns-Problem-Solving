#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

struct btnode    
{
    struct btnode *left;
    int data;
    struct btnode *right;
}btnode;

void insert(struct btnode ** , int );
void inorder(struct btnode *);
void preorder(struct btnode *sr);
void postorder(struct btnode *sr);
void search(struct btnode *, int);
void delete(struct btnode **, int);
void locate( struct btnode **, int , struct btnode **,struct btnode **, int *);