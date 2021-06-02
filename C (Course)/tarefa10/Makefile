CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: corretor

corretor: corretor.o hash.o
	gcc $(CFLAGS) corretor.o hash.o -o corretor $(LDFLAGS)

corretor.o: corretor.c hash.h
	gcc $(CFLAGS) -c corretor.c $(LDFLAGS)

hash.o: hash.c hash.h
	gcc $(CFLAGS) -c hash.c $(LDFLAGS)

clean:
	rm -f corretor
