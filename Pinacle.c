#include <stdio.h>
#include <string.h>


typedef struct Node
{
    int prn ;
    char name[15];
    struct Node *next;
}Node;

Node *createNode(int prn, char *name){
    Node *newNode = (Node*)malloc(sizeof(Node));
    
    if (newNode == NULL)
    {
        printf("MEMEORY FULL \n");
        exit(1);
    }

    newNode->prn = prn;
    strcpy(newNode->name,name);
    newNode->next = NULL;


    return newNode;
    
}
