CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: sala_de_espera

sala_de_espera: sala_de_espera.o deque.o
	gcc $(CFLAGS) sala_de_espera.o deque.o -o sala_de_espera $(LDFLAGS)

sala_de_espera.o: sala_de_espera.c deque.h
	gcc $(CFLAGS) -c sala_de_espera.c $(LDFLAGS)

deque.o: deque.c deque.h
	gcc $(CFLAGS) -c deque.c $(LDFLAGS)

clean:
	rm -f sala_de_espera
	rm -f deque
	rm -f *.o
