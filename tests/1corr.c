#include <stdio.h>
#include <stdlib.h>
#include "rand.h"
#include "hmproto.h"

#define ARRAYSZ(x) (sizeof(x)/sizeof(x[0]))

static int bad=0;
#define CHECK(x) do if (!(x)) printf("\e[31mWRONG: \e[1m%s\e[22m at line \e[1m%d\e[22m\n", #x, __LINE__),bad=1,exit(1); while (0)

static void test_smoke()
{
    void *c = hm_new();
    hm_insert(c, 123, (void*)456, 0);
    CHECK(hm_get(c, 123) == (void*)456);
    CHECK(hm_get(c, 124) == 0);
    hm_delete(c);
}

static void test_key0()
{
    void *c = hm_new();
    hm_insert(c, 1, (void*)1, 0);
    hm_insert(c, 0, (void*)2, 0);
    hm_insert(c, 65536, (void*)3, 0);
    CHECK(hm_get(c, 1)    == (void*)1);
    CHECK(hm_remove(c, 1) == (void*)1);
    CHECK(hm_get(c, 0)      == (void*)2);
    CHECK(hm_remove(c, 0)   == (void*)2);
    CHECK(hm_get(c, 65536)    == (void*)3);
    CHECK(hm_remove(c, 65536) == (void*)3);
    hm_delete(c);
}

static void test_1to1000()
{
    void *c = hm_new();
    for (long i=0; i<1000; i++)
        hm_insert(c, i, (void*)i, 0);
    for (long i=0; i<1000; i++)
        CHECK(hm_get(c, i) == (void*)i);
    hm_delete(c);
}

static void test_insert_delete1M()
{
    #define MAX 1048576
    void *c = hm_new();
    for (long i=0; i<MAX; i++)
    {
        CHECK(hm_get(c, i) == (void*)0);
        hm_insert(c, i, (void*)i, 0);
        CHECK(hm_get(c, i) == (void*)i);
        CHECK(hm_remove(c, i) == (void*)i);
        CHECK(hm_get(c, i) == (void*)0);
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
        CHECK(hm_get(c, i) == (void*)0);
        hm_insert(c, i, (void*)i, 0);
        CHECK(hm_get(c, i) == (void*)i);
    }
    for (long i=0; i<MAX; i++)
    {
        CHECK(hm_get(c, i) == (void*)i);
        CHECK(hm_remove(c, i) == (void*)i);
        CHECK(hm_get(c, i) == (void*)0);
    }
    hm_delete(c);
    #undef MAX
}

static void test_ffffffff_and_friends()
{
    static uintptr_t vals[]=
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
        CHECK(hm_get(c, vals[i]) == (void*)~vals[i]);
    for (int i=0; i<ARRAYSZ(vals); i++)
        CHECK(hm_remove(c, vals[i]) == (void*)~vals[i]);
    hm_delete(c);
}

static void test_insert_delete_random()
{
    void *c = hm_new();
    for (long i=0; i<1000000; i++)
    {
        uintptr_t v=rnd64();
        hm_insert(c, v, (void*)v, 0);
        CHECK(hm_get(c, v) == (void*)v);
        CHECK(hm_remove(c, v) == (void*)v);
        CHECK(hm_get(c, v) == 0);
    }
    hm_delete(c);
}

static void test_le_basic()
{
    void *c = hm_new();
#define INS(x) hm_insert(c, (x), (void*)(x), 0)
    INS(1);
    INS(2);
    INS(3);
    INS(0);
    INS(4);
    INS(0xf);
    INS(0xe);
    INS(0x11);
    INS(0x12);
    INS(0x20);
#define GET_SAME(x) CHECK(hm_get(c, (x)) == (void*)(x))
#define GET_NULL(x) CHECK(hm_get(c, (x)) == NULL)
    GET_NULL(122);
    GET_SAME(1);
    GET_SAME(2);
    GET_SAME(3);
    GET_SAME(4);
    GET_NULL(5);
    GET_SAME(0x11);
    GET_SAME(0x12);
#define LE(x,y) CHECK(hm_find_le(c, (x)) == (void*)(y))
    LE(1, 1);
    LE(2, 2);
    LE(5, 4);
    LE(6, 4);
    LE(0x11, 0x11);
    LE(0x15, 0x12);
    LE(0xfffffff, 0x20);
    hm_delete(c);
}

static uintptr_t expand_bits(uintptr_t x)
{
    return (x&0xc000)<<14
         | (x&0x3000)<<12
         | (x&0x0c00)<<10
         | (x&0x0300)<< 8
         | (x&0x00c0)<< 6
         | (x&0x0030)<< 4
         | (x&0x000c)<< 2
         | (x&0x0003);
}

static void test_le_brute()
{
    void *c = hm_new();
    char ws[65536]={0,};

    for (int cnt=0; cnt<1024; cnt++)
    {
        int w = mrand48()&0xffff;
        if (ws[w])
            hm_remove(c, expand_bits(w)), ws[w]=0;
        else
            hm_insert(c, expand_bits(w), (void*)expand_bits(w), 0), ws[w]=1;

        for (int cnt2=0; cnt2<1024; cnt2++)
        {
            w = mrand48()&0xffff;
            int v;
            for (v=w; v>=0 && !ws[v]; v--)
                ;
            uintptr_t res = (uintptr_t)hm_find_le(c, expand_bits(w));
            uintptr_t exp = (v>=0)?expand_bits(v):0;
            CHECK(res == exp);
        }
    }

    hm_delete(c);

}

static void test_same_only()
{
    void *c = hm_new();
    hm_insert(c, 123, (void*)456, 0);
    hm_insert(c, 123, (void*)457, 0);
    CHECK(hm_get(c, 123) == (void*)456);
    CHECK(hm_get(c, 124) == 0);
    hm_delete(c);
}

static void test_same_two()
{
    void *c = hm_new();
    hm_insert(c, 122, (void*)111, 0);
    hm_insert(c, 123, (void*)456, 0);
    hm_insert(c, 123, (void*)457, 0);
    CHECK(hm_get(c, 122) == (void*)111);
    CHECK(hm_get(c, 123) == (void*)456);
    CHECK(hm_get(c, 124) == 0);
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
    TEST(le_basic, 2);
    TEST(le_brute, 2);
    TEST(same_only, 2);
    TEST(same_two, 2);
    return 0;
}
