# doubly linked list

# singly linked list

struct _dnode {
        struct _dnode *previous;
        struct _dnode *next;
};
typedef struct _dnode dlnode_t;
 
struct _dlist {
        dlnode_t *head;
        dlnode_t *tail;
};
typedef struct _dlist dllist_t;