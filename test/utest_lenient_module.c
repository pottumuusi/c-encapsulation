#include <criterion/criterion.h>

#if 0
void setup(void) {
	    puts("Runs before the test");
}

void teardown(void) {
	    puts("Runs after the test");
}
#endif

Test(lenient_module, TODO_write_a_test_name)
{
    cr_expect(1 == 0);
}
