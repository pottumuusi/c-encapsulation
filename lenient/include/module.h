/*
 * A module for providing utilities for module management.
 *
 * To have visibility to `this` and `this_private` of any module during
 * compilation, 'function context' function definitions are placed behind
 * macros.
 *
 * TODO write what is expected from the file that generates functions using the
 * macros defined here.
 *
 * TODO implement tests.
 */

#ifndef MODULE_H_DEFINED
#define MODULE_H_DEFINED

#include <stddef.h>
#include <stdint.h>

#define MODULE_GENERATE_CONTEXT_FUNCTIONS(struct_public, struct_full) \
    MODULE_GENERATE_DECLARATIONS(struct_public)                       \
    MODULE_GENERATE_LOAD_CONTEXT(struct_public, struct_full)          \
    MODULE_GENERATE_UNLOAD_CONTEXT                                    \
    MODULE_GENERATE_VALIDATE_CONTEXT

#define MODULE_GENERATE_DECLARATIONS(struct_public)                \
    static int32_t module_load_context(struct_public* new_this);   \
    static void module_unload_context(void);                       \
    static void module_validate_context(void);

#define MODULE_GENERATE_LOAD_CONTEXT(struct_public, struct_full)           \
static int32_t                                                             \
module_load_context(struct_public* new_this)                               \
{                                                                          \
    if (NULL != this_public || NULL != this_private) {                     \
        fprintf(stderr, "Previous context has not been unloaded.");        \
        return -1;                                                         \
    }                                                                      \
                                                                           \
    if (NULL == new_this) {                                                \
        fprintf(stderr, "Function context load received a null pointer."); \
        return -1;                                                         \
    }                                                                      \
                                                                           \
    if (&(((struct_full*) new_this)->public) != new_this) {                \
        fprintf(                                                           \
            stderr,                                                        \
            "Function context load received an unrecognized pointer.");    \
        return -1;                                                         \
    }                                                                      \
                                                                           \
    this_public = new_this;                                                \
    this_private = &(((struct_full*) new_this)->private);                  \
                                                                           \
    return 0;                                                              \
}

#define MODULE_GENERATE_UNLOAD_CONTEXT \
static void                            \
module_unload_context(void)            \
{                                      \
    this_public = NULL;                \
    this_private = NULL;               \
}

#define MODULE_GENERATE_VALIDATE_CONTEXT \
static void                              \
module_validate_context(void)            \
{                                        \
    assert(NULL != this_public);         \
    assert(NULL != this_private);        \
}

#endif // MODULE_H_DEFINED
