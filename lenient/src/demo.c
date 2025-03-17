#include "demo.h"
#include "module.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static int add_to_foo_unrecoverable(int operand);
static int add_to_foo_recoverable(int operand, int* result);

// Module context begin
static struct demo* this_public;
static struct demo_private* this_private;
// Module context end

MODULE_GENERATE_CONTEXT_FUNCTIONS(struct demo, struct demo_full)

/*
 * Construct an instance of demo module to heap and return a pointer to public
 * data of the module.
 */
struct demo*
demo_construct_to_heap(int _foo)
{
    struct demo_full* instance_full;
    struct demo* instance_public;
    struct demo_private* instance_private;

    instance_full = NULL;

    /*
     * Allocate memory for a struct containing both public and private data of
     * demo module.
     */
    instance_full = calloc(1, sizeof(struct demo_full));
    if (NULL == instance_full) {
        fprintf(stderr, "Failed to allocate memory for full instance\n");
        return NULL;
    }

    // Extract public and private data from full module data.
    instance_public = &(instance_full->public);
    instance_private = &(instance_full->private);

    // Initialize public and private data.
    (void) instance_public;
    instance_private->foo = _foo;

#if DEBUG
    printf("demo_construct_to_heap, instance_full is: %p\n", instance_full);
    printf("demo_construct_to_heap, instance_public is: %p\n",
        instance_public);
    printf("demo_construct_to_heap, instance_private is: %p\n",
        instance_private);
#endif

    /*
     * Ensure that pointer to public data can be converted to a pointer of full
     * module data, that can be used for accessing module private data.
     */
    if ((void*) instance_full != (void*) instance_public) {
        fprintf(stderr,
            "Unexpected memory address for public field of module\n");
        free(instance_full);
        instance_full = NULL;
        instance_public = NULL;
        instance_private = NULL;
        return NULL;
    }

    return instance_public;
}

void
demo_destroy(struct demo* demo_to_destroy_public)
{
    struct demo_full* demo_to_destroy =
        (struct demo_full*) demo_to_destroy_public;

    free(demo_to_destroy);
    demo_to_destroy_public = NULL;
    demo_to_destroy = NULL;
}

int
demo_add_to_foo_unrecoverable(struct demo* instance_public, int operand)
{
    int err;
    int result;

    err = 0;
    result = 0;

    err = module_load_context(instance_public);
    if (0 != err) {
        fprintf(stderr, "Failed to load module context.\n");
        exit(1);
    }

    result = add_to_foo_unrecoverable(operand);
    module_unload_context();

    return result;
}

static int
add_to_foo_unrecoverable(int operand)
{
    module_exit_on_unloaded_context();

    return this_private->foo + operand;
}

int
demo_add_to_foo_recoverable(
    struct demo* instance_public,
    int operand,
    int* result)
{
    int err;

    err = 0;

    err = module_load_context(instance_public);
    if (0 != err) {
        fprintf(stderr, "Failed to load module context.\n");
        return -1;
    }

    err = add_to_foo_recoverable(operand, result);
    if (0 != err) {
        fprintf(stderr, "Failed to add to foo on internal level.\n");
        return -1;
    }

    module_unload_context();

    return 0;
}

static int
add_to_foo_recoverable(int operand, int* result)
{
    int err;

    err = 0;

    err = module_validate_context_loaded();
    if (0 != err) {
        fprintf(stderr, "Module context is not loaded while adding to foo\n");
        return -1;
    }

    *result = this_private->foo + operand;

    return 0;
}
