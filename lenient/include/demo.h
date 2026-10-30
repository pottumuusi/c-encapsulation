#ifndef DEMO_H_DEFINED
#define DEMO_H_DEFINED

struct demo {
};

struct demo_private {
    int foo;
};

struct demo_full {
    struct demo public;
    struct demo_private private;
};

struct demo* demo_construct_to_heap(int _foo);
void demo_destroy(struct demo* demo_to_destroy_public);

int demo_add_to_foo_unrecoverable(struct demo* instance_public, int operand);
int demo_add_to_foo_recoverable(
        struct demo* instance_public,
        int operand,
        int* result);

#endif // DEMO_H_DEFINED
