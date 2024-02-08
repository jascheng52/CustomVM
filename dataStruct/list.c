#include <stdlib.h>
#include <list.h>


//Adds node to back of list. Returns NULL if head is not sentinel node or new node is null
NODE *add_node(NODE *head, NODE *newNode)
{

    if(head == NULL || newNode == NULL || head->type != NONE)
        return NULL;

    NODE *prevNode = head->prev;
    prevNode->next = newNode;
    
    newNode->prev = prevNode;
    newNode->next = head;

    return newNode;
}

//Frees list from sentinel. Returns 1 on success. Should not use head again
int freeList(NODE *head)
{
    if(head == NULL || head->type != NONE)
        return -1;
    
    NODE *nextNode = head->next;
    while (nextNode != head)
    {
        NODE *current = nextNode;
        nextNode = nextNode->next;
        free(current);
    }
    free(head);
    return 1;
}