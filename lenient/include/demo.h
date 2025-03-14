#ifndef DEMO_H_DEFINED
#define DEMO_H_DEFINED

struct Demo {
};

struct DemoPrivate {
    int foo;
};

struct DemoFull {
    struct Demo public;
    struct DemoPrivate private;
};

struct Demo* demo_construct_to_heap(int _foo);
void demo_destroy(struct Demo* demo_to_destroy_public);

int demo_add_to_foo(struct Demo* object_public, int operand);

#endif // DEMO_H_DEFINED
