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

struct Demo*
demo_construct_to_heap(int _foo)
{
    struct DemoFull* object_full;
    struct Demo* object_public;
    struct DemoPrivate* object_private;

    object_full = NULL;

    object_full = calloc(1, sizeof(struct DemoFull));
    if (NULL == object_full) {
        fprintf(stderr, "Failed to allocate memory for full object\n");
        return NULL;
    }

    object_public = &(object_full->public);
    object_private = &(object_full->private);

    (void) object_public; // Here would initialize public data
    object_private->foo = _foo;

#if DEBUG
    printf("demo_construct_to_heap, object_full is: %p\n", object_full);
    printf("demo_construct_to_heap, object_public is: %p\n", object_public);
    printf("demo_construct_to_heap, object_private is: %p\n", object_private);
#endif

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
