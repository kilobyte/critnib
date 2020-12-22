#include <stdio.h>
#include "critnib.h"

int main()
{
    critnib *c = critnib_new();
    critnib_insert(c, 1, "one");
    critnib_insert(c, 2, "two");
    printf("1 → %s\n", (char*)critnib_get(c, 1));
    printf("2 → %s\n", (char*)critnib_get(c, 2));
    printf("≤1 → %s\n", (char*)critnib_find_le(c, 1));
    printf("≤2 → %s\n", (char*)critnib_find_le(c, 2));
    printf("≤3 → %s\n", (char*)critnib_find_le(c, 3));
    critnib_delete(c);

    return 0;
}
