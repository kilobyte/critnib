#include <stdio.h>
#include <stdlib.h>
#include "rand.h"
#include "hmproto.h"

#define ARRAYSZ(x) (sizeof(x)/sizeof(x[0]))

typedef uintptr_t word;

static int bad=0;
#define CHECK(x,y) do if ((x)!=(y)) \
    printf("\e[31mWRONG: \e[1m%s\e[22m (\e[1m%zx\e[22m) ≠ \e[1m%s\e[22m (\e[1m%zx\e[22m) at line \e[1m%d\e[0m\n", \
    #x, (uintptr_t)(x), #y, (uintptr_t)(y), __LINE__),bad=1,exit(1); while (0)

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
#define GET_SAME(x) CHECK(hm_get(c, (x)), (void*)(x))
#define GET_NULL(x) CHECK(hm_get(c, (x)), NULL)
    GET_NULL(122);
    GET_SAME(1);
    GET_SAME(2);
    GET_SAME(3);
    GET_SAME(4);
    GET_NULL(5);
    GET_SAME(0x11);
    GET_SAME(0x12);
#define LE(x,y) CHECK(hm_find_le(c, (x)), (void*)(y))
    LE(1, 1);
    LE(2, 2);
    LE(5, 4);
    LE(6, 4);
    LE(0x11, 0x11);
    LE(0x15, 0x12);
    LE(0xfffffff, 0x20);
    hm_delete(c);
}

static word expand_bits(word x)
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
        int w = rnd64()&0xffff;
        if (ws[w])
            hm_remove(c, expand_bits(w)), ws[w]=0;
        else
            hm_insert(c, expand_bits(w), (void*)expand_bits(w), 0), ws[w]=1;

        for (int cnt2=0; cnt2<1024; cnt2++)
        {
            w = rnd64()&0xffff;
            int v;
            for (v=w; v>=0 && !ws[v]; v--)
                ;
            word res = (word)hm_find_le(c, expand_bits(w));
            word exp = (v>=0)?expand_bits(v):0;
            CHECK(res, exp);
            void *W;
            if (hm_find(c, expand_bits(w), FIND_LE, 0, &W))
                res = (intptr_t)W;
            else
                res = 0;
            CHECK(res, exp);
        }
    }

    hm_delete(c);
}

static void test_ge_brute()
{
    void *c = hm_new();
    char ws[65536]={0,};

    for (int cnt=0; cnt<1024; cnt++)
    {
        int w = rnd64()&0xffff;
        if (ws[w])
            hm_remove(c, expand_bits(w)), ws[w]=0;
        else
            hm_insert(c, expand_bits(w), (void*)expand_bits(w), 0), ws[w]=1;

        for (int cnt2=0; cnt2<1024; cnt2++)
        {
            w = rnd64()&0xffff;
            int v;
            for (v=w; v<0x10000 && !ws[v]; v++)
                ;
            word res;
            word exp = (v<0x10000)?expand_bits(v):0;
            void *W;
            if (hm_find(c, expand_bits(w), FIND_GE, 0, &W))
                res = (intptr_t)W;
            else
                res = 0;
            CHECK(res, exp);
        }
    }

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
    TEST(le_basic, 2);
    TEST(le_brute, 2);
    TEST(ge_brute, 2);
    return 0;
}
