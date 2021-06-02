CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: diadeuber

diadeuber: diadeuber.o heap.o
	gcc $(CFLAGS) diadeuber.o heap.o -o diadeuber $(LDFLAGS)

diadeuber.o: diadeuber.c heap.h
	gcc $(CFLAGS) -c diadeuber.c $(LDFLAGS)

heap.o: heap.c heap.h
	gcc $(CFLAGS) -c heap.c $(LDFLAGS)

clean:
	rm -f diadeuber
	rm -f *.o
