ALL=m

CC=gcc
CFLAGS=-Wall -Og -g
LIBS=-pthread

all: $(ALL)

.c.o:
	$(CC) $(CFLAGS) -c $<
*.o:	critnib.h pmdk-compat.h

m: main.o critnib.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(ALL) *.o
