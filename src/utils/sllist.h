// singly linked list

// wrap the node content with a slnode_t into a struct

struct _snode {
        struct _snode *next;
};
typedef struct _snode slnode_t;
 
struct _slist {
        slnode_t *head;
        slnode_t *tail;
};
typedef struct _slist sllist_t;