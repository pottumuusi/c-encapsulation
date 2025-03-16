#include "demo.h"
#include "module.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static int add_to_foo(int operand);

// Module context begin
static struct Demo* this_public;
static struct DemoPrivate* this_private;
// Module context end

MODULE_GENERATE_CONTEXT_FUNCTIONS(struct Demo, struct DemoFull)

/*
 * Construct an instance of Demo module to heap and return a pointer to public
 * data of the module.
 */
struct Demo*
demo_construct_to_heap(int _foo)
{
    struct DemoFull* object_full;
    struct Demo* object_public;
    struct DemoPrivate* object_private;

    object_full = NULL;

    /*
     * Allocate memory for a struct containing both public and private data of
     * Demo module.
     */
    object_full = calloc(1, sizeof(struct DemoFull));
    if (NULL == object_full) {
        fprintf(stderr, "Failed to allocate memory for full object\n");
        return NULL;
    }

    // Extract public and private data from DemoFull.
    object_public = &(object_full->public);
    object_private = &(object_full->private);

    // Initialize public and private data.
    (void) object_public;
    object_private->foo = _foo;

#if DEBUG
    printf("demo_construct_to_heap, object_full is: %p\n", object_full);
    printf("demo_construct_to_heap, object_public is: %p\n", object_public);
    printf("demo_construct_to_heap, object_private is: %p\n", object_private);
#endif

    /*
     * Ensure that pointer to public data can be converted to a pointer of full
     * module data, that can be used for accessing private data.
     */
    if ((void*) object_full != (void*) object_public) {
        fprintf(stderr, "Unexpected memory address for public field of module\n");
        free(object_full);
        object_full = NULL;
        object_public = NULL;
        object_private = NULL;
        return NULL;
    }

    return object_public;
}

void
demo_destroy(struct Demo* demo_to_destroy_public)
{
    struct DemoFull* demo_to_destroy =
        (struct DemoFull*) demo_to_destroy_public;

    free(demo_to_destroy);
    demo_to_destroy_public = NULL;
    demo_to_destroy = NULL;
}

int
demo_add_to_foo(struct Demo* object_public, int operand)
{
    int result;

    result = 0;

    module_load_context(object_public);
    result = add_to_foo(operand);
    module_unload_context();

    return result;
}

static int
add_to_foo(int operand)
{
    module_exit_on_unloaded_context();

    return this_private->foo + operand;
}
