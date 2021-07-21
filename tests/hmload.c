#include <stdint.h>
#include <stdio.h>
#include "hmproto.h"

struct hm hms[] =
{
//  HM_ARR(critbit, 2),
//  HM_ARR(tcradix, 2),
    HM_ARR(critnib, 0),
//  HM_ARR(critnib_tag, 0),
};

void hm_select(int i)
{
    hm_new	= hms[i].hm_new;
    hm_delete	= hms[i].hm_delete;
    hm_insert	= hms[i].hm_insert;
    hm_remove	= hms[i].hm_remove;
    hm_get	= hms[i].hm_get;
    hm_find_le	= hms[i].hm_find_le;
    hm_find	= hms[i].hm_find;
    hm_name	= hms[i].hm_name;
    hm_immutable= hms[i].hm_immutable;
}
