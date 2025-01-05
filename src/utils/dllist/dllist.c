#include "dllist.h"

error_t dllist_init(dllist_t * const list) {
    dllist_node_init(&list->head);
}

error_t dllist_node_init(slnode_t * const node) {
    node->prev = node;
    node->next = node;
}

bool dllist_is_empty(dllist_t const * const list) {
    return list->head.next == &list->head;
}

error_t dllist_insert_after(slnode_t * const node, slnode_t * const new_node) {
    new_node->next = node->next;
    new_node->prev = node;
    node->next->prev = new_node;
    node->next = new_node;
}

error_t dllist_prepend(dllist_t const * const list, slnode_t * const new_node) {
    dllist_insert_after(new_node, &list->head);
}

error_t dllist_append(dllist_t const * const list, slnode_t * const new_node) {
    dllist_insert_after(&list->tail, node);
}

error_t dllist_remove(slnode_t * const node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    dllist_node_init(node); // Reset the node to prevent dangling pointers
}