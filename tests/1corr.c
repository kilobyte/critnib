#include <stdio.h>
#include <stdlib.h>
#include "rand.h"
#include "hmproto.h"

#define ARRAYSZ(x) (sizeof(x)/sizeof(x[0]))

typedef uintptr_t word;

static int bad=0;
#define CHECK(x,y) do if ((x)!=(y)) \
    printf("\e[31mWRONG: \e[1m%s\e[22m (\e[1m%zx\e[22m) ≠ \e[1m%s\e[22m (\e[1m%zx\e[22m) at line \e[1m%d\e[22m\n", \
    #x, (uintptr_t)(x), #y, (uintptr_t)(y), __LINE__),bad=1,exit(1); while (0)

static void test_smoke()
{
    void *c = hm_new();
    hm_insert(c, 123, (void*)456, 0);
    CHECK(hm_get(c, 123), (void*)456);
    CHECK(hm_get(c, 124), 0);
    hm_delete(c);
}

static void test_key0()
{
    void *c = hm_new();
    hm_insert(c, 1, (void*)1, 0);
    hm_insert(c, 0, (void*)2, 0);
    hm_insert(c, 65536, (void*)3, 0);
    CHECK(hm_get(c, 1)   , (void*)1);
    CHECK(hm_remove(c, 1), (void*)1);
    CHECK(hm_get(c, 0)     , (void*)2);
    CHECK(hm_remove(c, 0)  , (void*)2);
    CHECK(hm_get(c, 65536)   , (void*)3);
    CHECK(hm_remove(c, 65536), (void*)3);
    hm_delete(c);
}

static void test_1to1000()
{
    void *c = hm_new();
    for (long i=0; i<1000; i++)
        hm_insert(c, i, (void*)i, 0);
    for (long i=0; i<1000; i++)
        CHECK(hm_get(c, i), (void*)i);
    hm_delete(c);
}

static void test_insert_delete1M()
{
    #define MAX 1048576
    void *c = hm_new();
    for (long i=0; i<MAX; i++)
    {
        CHECK(hm_get(c, i), (void*)0);
        hm_insert(c, i, (void*)i, 0);
        CHECK(hm_get(c, i), (void*)i);
        CHECK(hm_remove(c, i), (void*)i);
        CHECK(hm_get(c, i), (void*)0);
    }
    hm_delete(c);
    #undef MAX
}

static void test_insert_bulk_delete1M()
{
    #define MAX (1048576)
    void *c = hm_new();
    for (long i=0; i<MAX; i++)
    {
        CHECK(hm_get(c, i), (void*)0);
        hm_insert(c, i, (void*)i, 0);
        CHECK(hm_get(c, i), (void*)i);
    }
    for (long i=0; i<MAX; i++)
    {
        CHECK(hm_get(c, i), (void*)i);
        CHECK(hm_remove(c, i), (void*)i);
        CHECK(hm_get(c, i), (void*)0);
    }
    hm_delete(c);
    #undef MAX
}

static void test_ffffffff_and_friends()
{
    static word vals[]=
    {
        0,
        0x7fffffff,
        0x80000000,
        0xffffffff,
#if __SIZEOF_SIZE_T__ == 8
        0x7fffffffFFFFFFFF,
        0x8000000000000000,
        0xFfffffffFFFFFFFF,
#endif
    };

    void *c = hm_new();
    for (int i=0; i<ARRAYSZ(vals); i++)
        hm_insert(c, vals[i], (void*)~vals[i], 0);
    for (int i=0; i<ARRAYSZ(vals); i++)
        CHECK(hm_get(c, vals[i]), (void*)~vals[i]);
    for (int i=0; i<ARRAYSZ(vals); i++)
        CHECK(hm_remove(c, vals[i]), (void*)~vals[i]);
    hm_delete(c);
}

static void test_insert_delete_random()
{
    void *c = hm_new();
    for (long i=0; i<1000000; i++)
    {
        word v=rnd64();
        hm_insert(c, v, (void*)v, 0);
        CHECK(hm_get(c, v), (void*)v);
        CHECK(hm_remove(c, v), (void*)v);
        CHECK(hm_get(c, v), 0);
    }
    hm_delete(c);
}

static void test_same_only()
{
    void *c = hm_new();
    hm_insert(c, 123, (void*)456, 0);
    hm_insert(c, 123, (void*)457, 0);
    CHECK(hm_get(c, 123), (void*)456);
    CHECK(hm_get(c, 124), 0);
    hm_delete(c);
}

static void test_same_two()
{
    void *c = hm_new();
    hm_insert(c, 122, (void*)111, 0);
    hm_insert(c, 123, (void*)456, 0);
    hm_insert(c, 123, (void*)457, 0);
    CHECK(hm_get(c, 122), (void*)111);
    CHECK(hm_get(c, 123), (void*)456);
    CHECK(hm_get(c, 124), 0);
    hm_delete(c);
}

static void run_test(void (*func)(void), const char *name, int req)
{
    printf("TEST: %s\n", name);
    for (int i=0; i<ARRAYSZ(hms); i++)
    {
        hm_select(i);
        if (hm_immutable & req)
        {
            printf(" \e[35m[\e[1m!\e[22m]\e[0m: %s\n", hm_name);
            continue;
        }
        printf(" \e[34m[\e[1m⚒\e[22m]\e[0m: %s\n", hm_name);
        bad=0;
        func();
        if (!bad)
            printf("\e[F \e[32m[\e[1m✓\e[22m]\e[0m\n");
    }
}
#define TEST(x,req) do run_test(test_##x, #x, req); while (0)

int main()
{
    randomize(-1);
    TEST(smoke, 0);
    TEST(key0, 0);
    TEST(1to1000, 0);
    TEST(insert_delete1M, 0);
    TEST(insert_bulk_delete1M, 0);
    TEST(ffffffff_and_friends, 0);
    TEST(insert_delete_random, 0);
    TEST(same_only, 2);
    TEST(same_two, 2);
    return 0;
}
