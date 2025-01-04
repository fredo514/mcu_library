typedef struct {
    mp_subsciber_uint32_t observer;
    bool is_valid;
    uint32_t value;
} pt_uint32_t;

typedef struct {
    uint32_t initial_value;
} pt_uint32_config_t;

error_t pt_uint32_init(pt_uint32_t * const mp, pt_uint32_config_t const* const config);
