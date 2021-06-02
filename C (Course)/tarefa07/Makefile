CFLAGS = -std=c99 -Wall -Werror -Werror=vla -g
LDFLAGS = -lm

.PHONY: all clean

all: mensageiro

mensageiro: mensageiro.o binary_tree.o
	gcc $(CFLAGS) mensageiro.o binary_tree.o -o mensageiro $(LDFLAGS)

mensageiro.o: mensageiro.c binary_tree.h
	gcc $(CFLAGS) -c mensageiro.c $(LDFLAGS)

binary_tree.o: binary_tree.c binary_tree.h
	gcc $(CFLAGS) -c binary_tree.c $(LDFLAGS)

clean:
	rm -f mensageiro
	rm -f *.o
