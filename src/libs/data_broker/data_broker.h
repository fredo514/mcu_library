#ifndef DATA_BROKER_H
#define DATA_BROKER_H

#include <stdbool.h>
#include <string.h>

#include "data_broker_congif.h"
#include "error.h"

typedef struct {
    const char* name;
    const char* type; // e.g. units, range, etc
    bool isValid;
    size_t seq_num;
    bool isLocked;
    void* change_cb;
    void* default_value;
    void* valuePtr;
} data_elem_t;

void broker_cb_attach(data_elem_id_t elem, void * cb);

// return error code if data invalid
// store value in container and return success if data valid
error_t broker_data_read(data_elem_id_t elem, void * container);

void broker_data_write(data_elem_id_t elem, void * val);

#endif // DATA_BROKER_H