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

// Test(sample, passing, .signal = SIGSEGV) {
#if 0
Test(lenient_module, module_wrapper_destructor)
{
}
#endif
