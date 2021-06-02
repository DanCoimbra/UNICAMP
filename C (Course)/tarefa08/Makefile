CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: listas_legais

listas_legais: listas_legais.o balanced_binary_tree.o
	gcc $(CFLAGS) listas_legais.o balanced_binary_tree.o -o listas_legais $(LDFLAGS)

listas_legais.o: listas_legais.c balanced_binary_tree.h
	gcc $(CFLAGS) -c listas_legais.c $(LDFLAGS)

balanced_binary_tree.o: balanced_binary_tree.c balanced_binary_tree.h
	gcc $(CFLAGS) -c balanced_binary_tree.c $(LDFLAGS)

clean:
	rm -f listas_legais
	rm -f *.o
