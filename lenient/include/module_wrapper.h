#ifndef MODULE_WRAPPER_H_DEFINED
#define MODULE_WRAPPER_H_DEFINED

struct module_wrapper {
    int exposed;
};

struct module_wrapper_private {
    int hidden;
};

struct module_wrapper_full {
    struct module_wrapper public;
    struct module_wrapper_private private;
};

struct module_wrapper*
module_wrapper_construct_to_heap(int _exposed, int _hidden);

void
module_wrapper_destroy(struct module_wrapper* target_public);

int32_t
module_wrapper_load_context(struct module_wrapper* new_this);

void
module_wrapper_unload_context(void);

int32_t
module_wrapper_validate_context_loaded(void);

void
module_wrapper_exit_on_unloaded_context(void);

#endif // MODULE_WRAPPER_H_DEFINED
