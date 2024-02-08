
//None is sentinel
typedef enum node_type{
    NONE,
    TOKEN,
    LABEL
}NODE_TYPE;


typedef struct listNode
{
    struct listNode *prev;
    struct listNode *next;
    NODE_TYPE type;
    char data[];
}NODE;

NODE *add_node(NODE *head, NODE *newNode);
int freeList(NODE *head);





