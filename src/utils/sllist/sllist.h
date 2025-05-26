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

error_t sllist_init(sllist_t * const list);
error_t sllist_node_init(slnode_t * const node);

bool sllist_is_empty(sllist_t const * const list);

error_t sllist_insert_after(slnode_t * const node, slnode_t * const new_node);
error_t sllist_prepend(sllist_t const * const list, slnode_t * const new_node);
error_t sllist_append(sllist_t const * const list, slnode_t * const new_node);

error_t sllist_remove(sllist_t * const list, size_t position);