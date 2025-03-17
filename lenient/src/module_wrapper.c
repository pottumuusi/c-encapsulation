#include "module.h"
#include "module_wrapper.h"

// Module context begin
static struct module_wrapper* this_public;
static struct module_wrapper_private* this_private;
// Module context end

MODULE_GENERATE_CONTEXT_FUNCTIONS(struct module_wrapper, struct module_wrapper_full)

struct module_wrapper*
module_wrapper_construct_to_heap(int _exposed, int _hidden)
{
    struct module_wrapper_full* instance_full;
    struct module_wrapper* instance_public;
    struct module_wrapper_private* instance_private;

    instance_full = NULL;
    instance_public = NULL;
    instance_private = NULL;

    instance_full = calloc(1, sizeof(struct module_wrapper_full));
    if (NULL == instance_full) {
        fprintf(stderr,
            "Failed to allocate memory for full module wrapper instance.");
        return NULL;
    }

    instance_public = &(instance_full->public);
    instance_private = &(instance_full->private);

    instance_public->exposed = _exposed;
    instance_private->hidden = _hidden;

    if ((void*) instance_full != (void*) instance_public) {
        fprintf(stderr,
            "Unexpected memory address for public field of module.\n");
        free(instance_full);
        instance_full = NULL;
        instance_public = NULL;
        instance_private = NULL;
        return NULL;
    }

    return instance_public;
}

void
module_wrapper_destroy(struct module_wrapper* target_public)
{
    struct module_wrapper_full* target =
        (struct module_wrapper_full*) target_public;

    free(target);
    target = NULL;
    target_public = NULL;
}

int32_t
module_wrapper_load_context(struct module_wrapper* new_this)
{
    return module_load_context(new_this);
}

void
module_wrapper_unload_context(void)
{
    module_unload_context();
}

int32_t
module_wrapper_validate_context_loaded(void)
{
    return module_validate_context_loaded();
}

void
module_wrapper_exit_on_unloaded_context(void)
{
    module_exit_on_unloaded_context();
}
