#include <stdint.h>

enum find_dir_t {
	FIND_L  = -2,
	FIND_LE = -1,
	FIND_EQ =  0,
	FIND_GE = +1,
	FIND_G  = +2,
};

#define HM_PROTOS(x) \
    void *x##_new(void);\
    void x##_delete(void *c);\
    \
    int x##_insert(void *c, uintptr_t key, void *value, int update);\
    void *x##_remove(void *c, uintptr_t key);\
    void *x##_get(void *c, uintptr_t key);\
    void *x##_find_le(void *c, uintptr_t key);\
    int x##_find(void *c, uintptr_t key, int dir, uintptr_t *rkey, void *rvalue);

//HM_PROTOS(critbit)
//HM_PROTOS(tcradix)
HM_PROTOS(critnib)
//HM_PROTOS(critnib_tag)

void *(*hm_new)(void);
void (*hm_delete)(void *c);
int (*hm_insert)(void *c, uintptr_t key, void *value, int update);
void *(*hm_remove)(void *c, uintptr_t key);
void *(*hm_get)(void *c, uintptr_t key);
void *(*hm_find_le)(void *c, uintptr_t key);
int (*hm_find)(void *c, uintptr_t key, int dir, uintptr_t *rkey, void *rvalue);
const char *hm_name;
int hm_immutable;

#define HM_SELECT_ONE(x,f) hm_##f=x##_##f
#define HM_SELECT(x) \
    HM_SELECT_ONE(x,new);\
    HM_SELECT_ONE(x,delete);\
    HM_SELECT_ONE(x,insert);\
    HM_SELECT_ONE(x,remove);\
    HM_SELECT_ONE(x,get);\
    HM_SELECT_ONE(x,find_le);\
    HM_SELECT_ONE(x,find);\
    hm_name=#x

#define HM_ARR(x,imm) { x##_new, x##_delete, x##_insert, x##_remove, x##_get, \
                        x##_find_le, x##_find, #x, imm }
struct hm
{
    void *(*hm_new)(void);
    void (*hm_delete)(void *c);
    int (*hm_insert)(void *c, uintptr_t key, void *value, int update);
    void *(*hm_remove)(void *c, uintptr_t key);
    void *(*hm_get)(void *c, uintptr_t key);
    void *(*hm_find_le)(void *c, uintptr_t key);
    int (*hm_find)(void *c, uintptr_t key, int dir, uintptr_t *rkey, void *rvalue);
    const char *hm_name;
    int hm_immutable;
} hms[1];

void hm_select(int i);
