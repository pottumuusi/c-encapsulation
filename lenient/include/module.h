/*
 * A module for providing utilities for module management.
 *
 * The file where MODULE_GENERATE_CONTEXT_FUNCTIONS() is called is expected to
 * contain static struct pointer variables `this_public` and `this_private`.
 * Type of `this_public` should be the one whose pointer gets returned in a
 * constructor. It is expected that `this_private` is not exposed outside the
 * compilation unit via a pointer for example.
 *
 * Each function utilizing `this_public` or `this_private` should first call
 * `module_load_context()` before use of the variables and finish by calling
 * `module_unload_context()` when done using the variables.
 * `module_is_context_loaded()` and `module_exit_on_unloaded_context()` are
 * provided for sanity checking that a context is actually loaded and as an
 * example can be utilized in internal functions if a file provides exposed
 * functions calling to internal functions.
 *
 * To have visibility to static `this_public` and `this_private` of any module
 * during compilation, 'module context' function definitions are placed behind
 * macros.
 */

#ifndef MODULE_H_DEFINED
#define MODULE_H_DEFINED

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MODULE_GENERATE_CONTEXT_FUNCTIONS(struct_public, struct_full) \
    MODULE_GENERATE_DECLARATIONS(struct_public)                       \
    MODULE_GENERATE_LOAD_CONTEXT(struct_public, struct_full)          \
    MODULE_GENERATE_UNLOAD_CONTEXT                                    \
    MODULE_GENERATE_VALIDATE_CONTEXT_LOADED                           \
    MODULE_GENERATE_EXIT_ON_UNLOADED_CONTEXT

#define MODULE_GENERATE_DECLARATIONS(struct_public)                \
    static int32_t module_load_context(struct_public* new_this);   \
    static void module_unload_context(void);                       \
    static int32_t module_is_context_loaded(void);                 \
    static void module_exit_on_unloaded_context(void);

/*
 * Returns -1 on error and 0 on success.
 */
#define MODULE_GENERATE_LOAD_CONTEXT(struct_public, struct_full)             \
static int32_t                                                               \
module_load_context(struct_public* new_this)                                 \
{                                                                            \
    if (NULL != this_public || NULL != this_private) {                       \
        fprintf(stderr, "Previous module context has not been unloaded.\n"); \
        return -1;                                                           \
    }                                                                        \
                                                                             \
    if (NULL == new_this) {                                                  \
        fprintf(stderr, "Module context load received a null pointer.\n");   \
        return -1;                                                           \
    }                                                                        \
                                                                             \
    if (&(((struct_full*) new_this)->public) != new_this) {                  \
        fprintf(                                                             \
            stderr,                                                          \
            "Module context load received an unrecognized pointer.\n");      \
        return -1;                                                           \
    }                                                                        \
                                                                             \
    this_public = new_this;                                                  \
    this_private = &(((struct_full*) new_this)->private);                    \
                                                                             \
    return 0;                                                                \
}

#define MODULE_GENERATE_UNLOAD_CONTEXT \
static void                            \
module_unload_context(void)            \
{                                      \
    this_public = NULL;                \
    this_private = NULL;               \
}

/*
 * Returns 1 if context is considered to be loaded and returns 0 otherwise.
 */
#define MODULE_GENERATE_VALIDATE_CONTEXT_LOADED   \
static int32_t                                    \
module_is_context_loaded(void)                    \
{                                                 \
    if (NULL == this_public) {                    \
        return 0;                                 \
    }                                             \
                                                  \
    if (NULL == this_private) {                   \
        return 0;                                 \
    }                                             \
                                                  \
    return 1;                                     \
}

// TODO [#20] To allow testing, consider if a signal can be fired instead of
// exiting.
#define MODULE_GENERATE_EXIT_ON_UNLOADED_CONTEXT           \
static void                                                \
module_exit_on_unloaded_context(void)                      \
{                                                          \
    if ( ! module_is_context_loaded()) {                   \
        fprintf(stderr, "Exiting on unloaded context.\n"); \
        exit(1);                                           \
    }                                                      \
}

#endif // MODULE_H_DEFINED
