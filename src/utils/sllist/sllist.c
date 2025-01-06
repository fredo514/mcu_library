#include "sllist.h"

error_t sllist_init(sllist_t * const list) {
    sllist_node_init(list->head);
}

error_t sllist_node_init(slnode_t * const node) {
    node->next = node;
}

bool sllist_is_empty(sllist_t const * const list) {
    return list->head.next == list->head;
}

error_t sllist_insert_after(slnode_t * const node, slnode_t * const new_node) {
    new_node->next = node->next;
    node->next = new_node;
}

error_t sllist_prepend(sllist_t const * const list, slnode_t * const new_node) {
    sllist_insert_after(new_node, list->head);
}

error_t sllist_append(sllist_t const * const list, slnode_t * const new_node) {
    sllist_insert_after(list->tail, node);
}

error_t sllist_remove(sllist_t * const list, size_t position) {
    slnode_t* previous_node;
    slnode_t* current_node = list->head;
    
    // Traverse till given position
    for (int i = 0; (i != position) && (current_node->next != NULL); ++i) {
        previous_node = temp;
        current_node = current_node->next;
    }

    // If given position is found, delete node
    if (temp != NULL) {
        previous_node->next = current_node->next;
        sllist_node_init(current_node); // Reset the node to prevent dangling pointers
    }
    else {
        return ERROR_OUT_OF_RANGE;
    }

    return NO_ERROR;
}