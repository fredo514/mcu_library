#ifndef DLLIST_H
#define DLLIST_H

#include "error.h"

typedef struct _dnode {
        struct _dnode *previous;
        struct _dnode *next;
        void * content;
} dlnode_t;
 
typedef struct _dlist {
        dlnode_t *head;
} dllist_t;

error_t dllist_init(dllist_t * const list);
error_t dllist_node_init(dlnode_t * const node, void const * const content);

bool dllist_is_empty(dllist_t const * const list);
size_t dllist_num_nodes_get(dllist_t const * const list);
bool dllist_is_node_in_list(dllist_t const * const list, dlnode_t const * const node);

error_t dllist_node_insert_after(dlnode_t * const node, dlnode_t * const new_node);
error_t dllist_node_insert_before(dlnode_t * const node, dlnode_t * const new_node);
error_t dllist_node_insert_first(dllist_t const * const list, dlnode_t * const new_node);
error_t dllist_node_insert_last(dllist_t const * const list, dlnode_t * const new_node);
error_t dllist_node_remove(dlnode_t * const node);
dlnode_t dllist_node_previous_get(dlnode_t * const node);
dlnode_t dllist_node_next_get(dlnode_t * const node);
void* dllist_node_content_get(dlnode_t * const node);

#endif // DLLIST_H
