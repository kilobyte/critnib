#include <critnib.h>
#include <stdio.h>

static int bad=0;

#define PUT(x) if (critnib_insert(c, (x), (void*)(uintptr_t)(x), 0)) \
    {fprintf(stderr, "insert(%zu) failed\n", (uintptr_t)(x));bad=1;}
#define CHECK(op,x,y) {uintptr_t z=(uintptr_t)critnib_##op(c,x); \
    if ((y)!=z) {fprintf(stderr, "FAIL: %s(%zu) should be %zu is %zu\n", #op, (uintptr_t)(x), (uintptr_t)(y), z);bad=1;}}

int main()
{
    critnib *c = critnib_new();
    PUT(1);
    PUT(2);
    PUT(17);
    CHECK(get, 1, 1);
    CHECK(get, 2, 2);
    CHECK(get, 3, 0);
    CHECK(get, 16, 0);
    CHECK(get, 17, 17);
    CHECK(find_le, 1, 1);
    CHECK(find_le, 5, 2);
    CHECK(find_le, -1, 17);
    CHECK(remove, 3, 0);
    CHECK(remove, 17, 17);
    CHECK(find_le, -1, 2);
    critnib_delete(c);
    return bad;
}
