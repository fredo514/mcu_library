// singly linked list

// wrap the node content with a slnode_t into a struct

typedef struct _snode {
        struct _snode *next;
        void* content;
} slnode_t;
 
typedef struct _slist {
        slnode_t *head;
        slnode_t *tail;
} sllist_t;