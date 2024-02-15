#include <stdlib.h>
#include <stdio.h>
#include <list.h>


//Adds node to back of list. Returns NULL if head is not sentinel node or new node is null
NODE *LIST_add_node(NODE *head, NODE *newNode)
{

    if(head == NULL || newNode == NULL || head->type != NONE)
        return NULL;

    NODE *prevNode = head->prev;
    prevNode->next = newNode;
    
    newNode->prev = prevNode;
    newNode->next = head;
    head->prev = newNode;

    return newNode; 
}


//Prints pointer link
void LIST_print(NODE *head)
{
    NODE *next = head->next;
    printf("HEAD: %p\n", head);

    int n = 1;
    while (next != head)
    {
        printf("%d: %p\n",n,next);
        next = next->next;
        n++;
    }
    printf("END: %p\n",next);
    fflush(stdout);
    
}