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

int main(){
    struct btnode *bt;
    int i=0 , arr[] = {2,8,10,16,18,24,26,30};
    bt = NULL;
    int len = sizeof(arr)/sizeof(arr[0]);
    while (i<=len)
    {
        insert(&bt, arr[i]);
        i++;
    }
    
}

void insert(struct btnode **sr, int data){

    if (*sr == NULL)
    {
        *sr = (struct btnode *)malloc(sizeof( struct btnode ));
        (*sr)->left = NULL;
        (*sr)->data = data;
        (*sr)->right = NULL;
    }
    else
    {
        if (data < (*sr)->data)
        {
            (*sr) = (*sr)->left;
            insert(&(*sr),data);
        }
        else
        {
            (*sr)= (*sr)->right;
            insert(&(*sr),data);
        }
        
        
    }
    
    
}