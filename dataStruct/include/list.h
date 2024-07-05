#ifndef LIST_H
#define LIST_H
//None is sentinel
typedef enum node_type{
    NONE,
    DATA,
    LABEL,
    BLOCK,
    INSTR
}NODE_TYPE;

//data depends on type
typedef struct listNode
{
    struct listNode *prev;
    struct listNode *next;
    NODE_TYPE type;
    void *data;
}NODE;

NODE *LIST_add_node(NODE *head, NODE *newNode);
int LIST_free_list(NODE *head);
void LIST_print(NODE *head);


#endif /*LIST.H*/





