CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: caminho

caminho: caminho.o grafo.o
	gcc $(CFLAGS) caminho.o grafo.o -o caminho $(LDFLAGS)

caminho.o: caminho.c grafo.h
	gcc $(CFLAGS) -c caminho.c $(LDFLAGS)

grafo.o: grafo.c grafo.h
	gcc $(CFLAGS) -c grafo.c $(LDFLAGS)

clean:
	rm -f caminho
	rm -f *.o
