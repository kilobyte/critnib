#include <stdio.h>
#include <string.h>
#include "critnib.h"

int main()
{
    critnib *c = critnib_new();
    critnib_insert(c, 1, "one", 0);
    critnib_insert(c, 2, "bad", 0);
    printf("overwrite(, 0) → %s\n", strerror(critnib_insert(c, 2, "foo", 0)));
    printf("overwrite(, 1) → %s\n", strerror(critnib_insert(c, 2, "two", 1)));
    printf("1 → %s\n", (char*)critnib_get(c, 1));
    printf("2 → %s\n", (char*)critnib_get(c, 2));
    printf("≤1 → %s\n", (char*)critnib_find_le(c, 1));
    printf("≤2 → %s\n", (char*)critnib_find_le(c, 2));
    printf("≤3 → %s\n", (char*)critnib_find_le(c, 3));
    critnib_delete(c);

    return 0;
}
