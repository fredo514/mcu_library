// doubly linked list

// wrap the node content with a slnode_t into a struct

typedef struct _dnode {
        struct _dnode *previous;
        struct _dnode *next;
} dlnode_t;
 
typedef struct _dlist {
        dlnode_t *head;
        dlnode_t *tail;
} dllist_t;

error_t dllist_init(dllist_t * const list);
error_t dllist_node_init(dlnode_t * const node);

bool dllist_is_empty(dllist_t const * const list);

error_t dllist_insert_after(dlnode_t * const node, dlnode_t * const new_node);
error_t dllist_prepend(dllist_t const * const list, dlnode_t * const new_node);
error_t dllist_append(dllist_t const * const list, dlnode_t * const new_node);

error_t dllist_remove(dlnode_t * const node);