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
int search(struct btnode *, int);
void delete(struct btnode **, int);
void locate(struct btnode **, int, struct btnode **, struct btnode **, int *);

int main()
{
    struct btnode *bt;
    int i = 0, arr[] = {2, 60, 10, 31, 45, 24, 26, 90};
    bt = NULL;
    int flag = 0;
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
    printf("\n Postorder : ");
    postorder(bt);

    int val;
    printf("\n Enter The Value You Want to Search :");
    scanf("%d", &val);

    flag = search(bt, val);
    if (flag == 1)
    {
        printf("\n %d Found !");
    }
    else
    {
        printf("\n %d NOT Found !");
    }
    int del;
    printf("\n Enter The Value You Want to delete :");
    scanf("%d", &del);
    delete(&bt, del);
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
void postorder(struct btnode *sr)
{
    if (sr != NULL)
    {
        postorder(sr->left);
        postorder(sr->right);
        printf(" %d \t", sr->data);
    }
}
int search(struct btnode *sr, int val)
{
    while (sr != NULL)
    {
        if (val == sr->data)
        {
            return 1;
        }
        else if (val < sr->data)
        {
            sr = sr->left;
        }
        else
        {
            sr = sr->right;
        }
    }
    return 0;
}
void delete(struct btnode **root, int del)
{
    int found;
    struct btnode *parent, *x, *xs;

    if (root = NULL)
    {
        printf("TREE IS EMPTY \n");
        return;
    }

    parent = x = NULL;

    locate(root, del, &parent, &x, &found);

    if(found == FALSE){
        printf("\n NODE not FOUND !");
        return;
    }

    // del with node having two child 

    if(x->left != NULL && x->right != NULL){
        parent = x;
        xs = x->right;
        while (xs->left != NULL)
        {
            parent = xs;
            xs = xs->left;
        }
        x->data = xs->data;
        x = xs;
        
    }
}

void locate(struct btnode **root, int del, struct btnode **par, struct btnode **x, int *found)
{

    struct btnode *temp;
    temp = *root;
    *found = FALSE;
    *par = NULL;

    while (temp != NULL)
    {
        if (temp->data == del)
        {
            *x = temp;
            *found = TRUE;
            return;
        }
        *par = temp;
        if (temp->data > del)
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
    }
}