#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "module_wrapper.h"

#if 0
void setup(void) {
	    puts("Runs before the test");
}

void teardown(void) {
	    puts("Runs after the test");
}

Test(sample, passing, .signal = SIGSEGV)
{
}
#endif

Test(lenient_module, module_wrapper_constructor_returns_an_instance)
{
    struct module_wrapper* module_wrapper;

    module_wrapper = NULL;

    module_wrapper = module_wrapper_construct_to_heap(1, 2);
    cr_assert(ne(module_wrapper, NULL));

    module_wrapper_destroy(module_wrapper);
    module_wrapper = NULL;
}

Test(lenient_module, context_load_succeeds_for_instance_public_pointer)
{
    int err;
    struct module_wrapper* module_wrapper;

    err = -1;
    module_wrapper = NULL;

    module_wrapper = module_wrapper_construct_to_heap(1, 2);
    err = module_wrapper_load_context(module_wrapper);
    cr_expect(eq(err, 0));

    module_wrapper_destroy(module_wrapper);
    module_wrapper = NULL;
}

Test(lenient_module, context_load_fails_for_null_pointer)
{
    int err;

    err = -1;

    err = module_wrapper_load_context(NULL);
    cr_expect(ne(err, 0));
}

Test(lenient_module, context_load_load_result_in_fail)
{
    int err;
    struct module_wrapper* module_wrapper;

    err = -1;
    module_wrapper = NULL;

    module_wrapper = module_wrapper_construct_to_heap(1, 2);
    err = module_wrapper_load_context(module_wrapper);
    cr_assert(eq(err, 0));

    err = module_wrapper_load_context(module_wrapper);
    cr_assert(ne(err, 0));

    module_wrapper_destroy(module_wrapper);
    module_wrapper = NULL;
}

Test(lenient_module, context_load_unload_load_result_in_success)
{
    int err;
    struct module_wrapper* module_wrapper;

    err = -1;
    module_wrapper = NULL;

    module_wrapper = module_wrapper_construct_to_heap(1, 2);
    err = module_wrapper_load_context(module_wrapper);
    cr_assert(eq(err, 0));

    module_wrapper_unload_context();
    err = module_wrapper_load_context(module_wrapper);
    cr_assert(eq(err, 0));

    module_wrapper_destroy(module_wrapper);
    module_wrapper = NULL;
}

Test(lenient_module, context_validated_to_not_be_loaded_before_load)
{
    int err;
    int is_loaded;
    struct module_wrapper* module_wrapper;

    err = -1;
    is_loaded = -1;
    module_wrapper = NULL;

    /*
     * Constructed for checking that there is no side effect to context load
     * validation.
     */
    module_wrapper = module_wrapper_construct_to_heap(1, 2);

    is_loaded = module_wrapper_is_context_loaded();
    cr_assert(zero(i32, is_loaded));

    module_wrapper_destroy(module_wrapper);
    module_wrapper = NULL;
}

Test(lenient_module, context_validated_to_be_loaded_after_load)
{
    int err;
    int is_loaded;
    struct module_wrapper* module_wrapper;

    err = -1;
    is_loaded = -1;
    module_wrapper = NULL;

    module_wrapper = module_wrapper_construct_to_heap(1, 2);
    err = module_wrapper_load_context(module_wrapper);
    cr_assert(eq(err, 0));

    is_loaded = module_wrapper_is_context_loaded();
    cr_assert(eq(i32, is_loaded, 1));

    module_wrapper_destroy(module_wrapper);
    module_wrapper = NULL;
}

Test(lenient_module, context_validated_to_not_be_loaded_after_unload)
{
    int err;
    int is_loaded;
    struct module_wrapper* module_wrapper;

    err = -1;
    is_loaded = -1;
    module_wrapper = NULL;

    module_wrapper = module_wrapper_construct_to_heap(1, 2);
    err = module_wrapper_load_context(module_wrapper);
    cr_assert(eq(err, 0));

    module_wrapper_unload_context();
    is_loaded = module_wrapper_is_context_loaded();
    cr_assert(zero(i32, is_loaded));

    module_wrapper_destroy(module_wrapper);
    module_wrapper = NULL;
}
