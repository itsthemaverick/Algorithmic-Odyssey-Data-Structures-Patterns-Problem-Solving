#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

struct btnode
{
    struct btnode *left;
    int data;
    struct btnode *right;
};

void insert(struct btnode **, int);
void inorder(struct btnode *);
void preorder(struct btnode *sr);
void postorder(struct btnode *sr);
void search(struct btnode *, int);
void delete(struct btnode **, int);
void locate(struct btnode **, int, struct btnode **, struct btnode **, int *);

int main()
{
    struct btnode *bt;
    int i = 0, arr[] = {2, 60, 10, 31, 45, 24, 26, 90};
    bt = NULL;
    int len = sizeof(arr) / sizeof(arr[0]);
    while (i <= len - 1)
    {
        insert(&bt, arr[i]);
        i++;
    }
    printf("BST after Insertion is : ");
    printf("\n Inorder : ");
    inorder(bt);
    printf("\n Preorder : ");
    preorder(bt);
}

void insert(struct btnode **sr, int data)
{

    if (*sr == NULL)
    {
        *sr = (struct btnode *)malloc(sizeof(struct btnode));
        (*sr)->left = NULL;
        (*sr)->data = data;
        (*sr)->right = NULL;
    }
    else
    {
        if (data < (*sr)->data)
        {
            insert(&((*sr)->left), data);
        }
        else
        {
            insert(&((*sr)->right), data);
        }
    }
}
void inorder(struct btnode *sr)
{
    if (sr != NULL)
    {
        inorder(sr->left);
        printf(" %d \t", sr->data);
        inorder(sr->right);
    }
}
void preorder(struct btnode *sr)
{
    if (sr != NULL)
    {
        printf(" %d \t", sr->data);
        preorder(sr->left);
        preorder(sr->right);
    }
}