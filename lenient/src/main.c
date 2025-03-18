/*
 * Scetching the idea of how to implement private data for modules.
 *
 * demo_test_addToFoo(demo_test);
 *
 * demo_test.addToFoo();
 * demo_test.addToFoo(demo_test, 2);
 * demo_test.foo; // This should produce an error.
 *
 * Having `demo_test` twice in the statement looks clumsy. It would be
 * needed to write demo* as the first parameter of every function.
 *
 * ENTER_CONTEXT(demo_test)
 * demo_test.addToFoo();
 * EXIT_CONTEXT()
 *
 * How to assure exit for every enter? One way to assure is to grep and
 * check that there are as many enter calls as there are exit calls. Again
 * the repeated `demo_test` looks clumsy. The repetition may look clumsy,
 * but it simultaneously acts as a sanity check. I.e. should enter
 * `demo_test` context when proceeding to run `demo_test` functions.
 *
 * INVOKE_MODULE_FUNCTION(
 *     demo_test,
 *     addToFoo,
 *     2)
 * -> demo_test.addToFoo(demo_test, 2)
 *
 * Here it would be needed to write demo* as the first parameter of every
 * function.
 *
 * SET_MODULE(demo_test);
 * SET_FUNCTION(addToFoo);
 * SET_ARGUMENTS(2);
 * INVOKE_DEMO_FUNCTION(); // Should finish by clearing the context.
 *
 * Pass the arguments using heap memory instead of giving to function
 * call arguments.
 *
 * The arguments should be cast to needed types in the invoked function.
 * How to ensure type safety? In this case the arguments need allocated
 * memory. How much should be allocated?
 *
 * One solution is to have a public interface, which takes module struct
 * as a parameter, sets up this and calls an internal interface, which
 * is devoid of the struct.
 *
 * A simple solution is to have public functions in modules. The idea of always
 * exposing functions via fields of struct is to be scrapped (nope, later
 * decided to stick with functions in struct, but there is a single interface
 * struct instead of having the function pointers in every instance of a
 * module). The module containing a function can be detected from the struct
 * passed as an argument. The struct argument also protects from identical
 * function signatures in two different modules. A policy for designing is to
 * always pass struct pointer pointing to module data to a module function. If
 * the function does not need module data, then the function does not belong to
 * the module. This applies to modules which are instantiated more than once.
 *
 * It would seem to align with C design to separate the interface and data from
 * each other. So to provide functions which take module instances as
 * parameters and operate with their data. The instantiable module definition
 * should not contain any behavior, only data. The resulting call site could
 * look like the following:
 *
 * demo->add_to_foo(demo_instance, 123);
 *
 * The signature of the function can be simplified (in an attempt to make it
 * easier to recall purpose of functions from minimal set of parameters) by
 * writing an internal function with reduced parameters, the instance being
 * passed in some other way, such as static global variable. And here the
 * information hiding comes into play. Private data of the instance is made
 * easily accessible in the internal function. When private data is implemented
 * as a field of demo_full and demo is returned from the constructor, it is
 * possible to access private data outside the module. Accessing private data
 * outside its module is against design principles. I deem this private data
 * hiding to be sufficient to prevent accidental use outside their modules.
 *
 * If it is wanted to prevent use of private data alltogether outside their
 * modules, return public instance pointer, have a static list of full module
 * instances containing pointer to both public and private instance and have a
 * function for finding private counterpart of the public instance by accessing
 * the list of full module instances. As a drawback, this should prevent the
 * possibility to find private counterpart for instances constructed with
 * automatic storage (stored to stack), as demo_full containing demo_private fall
 * out of scope when returning from constructor.
 * https://en.wikipedia.org/wiki/C_syntax#Storage_class_specifiers
 *
 * I came across a set of guidelines for developing safe C code: "The Power of
 * Ten – Rules for Developing Safety Critical Code". The text can be found
 * from: https://spinroot.com/gerard/pdf/P10.pdf. In the guidelines, use of
 * function pointers is prohibited. I am going to conform and remove the use of
 * function pointers in module interfacing, as they are not strictly necessary.
 * In a game project I may still utilize function pointers as they can reduce
 * code size due to their power of expression. When removing function pointer
 * use, the module call becomes:
 *
 * demo_add_to_foo(demo_instance, 123);
 */

#include "demo.h"

#include <stddef.h>
#include <stdio.h>

// Define:
// * DEBUG to be 1 in compiler flags for enabling debug features.
// * ACCESS_PRIVATE_VIA_PUBLIC_INTERFACE to be 1 for producing a compilation error.

int main(void)
{
    struct demo* instance_demo;

    int err;
    int result;

    err = 0;
    result = 0;
    instance_demo = NULL;

    instance_demo = demo_construct_to_heap(4);
    if (NULL == instance_demo) {
        fprintf(stderr, "Failed to construct demo instance.");
        return 1;
    }

    result = demo_add_to_foo_unrecoverable(instance_demo, 44);
    printf(
        "After calling demo_add_to_foo_unrecoverable, result is: %d\n",
        result);

    err = demo_add_to_foo_recoverable(instance_demo, 26, &result);
    if (0 != err) {
        fprintf(stderr, "Failed to add to foo.\n");
    } else {
        printf(
            "After calling demo_add_to_foo_recoverable, result is: %d\n",
            result);
    }

#if ACCESS_PRIVATE_VIA_PUBLIC_INTERFACE
    printf("instance_demo->foo is: %d", instance_demo->foo);
#endif

    demo_destroy(instance_demo);
    instance_demo = NULL;

    return 0;
}
