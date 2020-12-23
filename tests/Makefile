ALL=1corr th
OBJ=hmload.o ../critnib.o

CC=gcc
CFLAGS=-Wall -g -O3 -pthread -fcommon

all: $(ALL)

.c.o:
	$(CC) $(CFLAGS) -c $<

*.o:	hmproto.h

clean:
	rm -f $(ALL) *.o

1corr: 1corr.o $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

th: th.o $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
