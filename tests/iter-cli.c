#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "../critnib.h"

#define die(...) do {fprintf(stderr, __VA_ARGS__); exit(1);} while(0)

static int itf(uintptr_t key, void *value, void *dummy)
{
	printf("%zu\n", key);
	return (key == 42);
}

int main(int argc, char **argv)
{
	if (argc != 3)
		die("Need two args: min, max\n");
	uintptr_t min = atol(argv[1]);
	uintptr_t max = atol(argv[2]);

	critnib *c = critnib_new();

	uintptr_t x;
	int err;
	while (scanf("%zu", &x) == 1)
		if ((err = critnib_insert(c, x, (void*)x, 0))) {
			if (err == EEXIST)
				printf("dupe: %zu\n", x);
			else
				die("critnib_insert failed\n");
		}

	critnib_iter(c, min, max, itf, 0);

	critnib_delete(c);
	return 0;
}
