#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hash.h"

// Aloca dinamicamente a memória para um nó de uma ligada que se situará numa hashtable (por isso 'hashlist').
// Cada nó da hashlist armazena a chave que levou à sua criação. Todos os nós em uma hashlist possuem chaves
// com o mesmo hash, mas tais chaves podem ser diferentes devido à imperfeição da hashtable escolhida.
hlist *cria_hlist(char *key, hlist *next) {
	hlist *hashlist = malloc(sizeof(hlist));
	if (hashlist == NULL)
		exit(1);
	memcpy((*hashlist).key, key, KEYSIZE);
	free(key);
	(*hashlist).next = next;
	return hashlist;
}

// Aloca dinamicamente a memória para uma hashtable contendo TABLESIZE hashes diferentes, cada um dos quais
// pode ser preenchido por uma lista ligada (e portanto colisões são administradas por endereçamento fechado).
	htable *cria_htable() {
	htable *hashtable = malloc(sizeof(htable));
	if (hashtable == NULL)
		exit(1);
	for (int i = 0; i < TABLESIZE; i++)
		(*hashtable).keylist[i] = NULL;
	return hashtable;
}

// Iterativamente libera a memória dinamicamente alocada para uma hashlist.
void free_hlist(hlist *hashlist) {
	hlist *ptr_hashlist = hashlist;
	while (ptr_hashlist != NULL) {
		hlist *next = (*ptr_hashlist).next;
		free(ptr_hashlist);
		ptr_hashlist = next;
	}
	return;
}

// Iterativamente libera a memória dinamicamente alocada para cada uma das hashlists na hashtable e, então, para a própria hashtable.
// A memória dinamicamente alocada para o atributo 'keylist' é liberada no processo de liberar as hashlists.
void free_htable(htable *hashtable) {
	for (int i = 0; i < TABLESIZE; i++)
		free_hlist((*hashtable).keylist[i]);
	free(hashtable);
	return;
}

// Insere uma hashlist (nó de lista ligada) no seu hash correspondente em uma hashtable. A inserção ocorre no começo da lista ligada.
// Copia a string 'key' para este nó e libera a memória dinamicamente alocada para a string original.
void insere_htable(htable *hashtable, char *key) {
	(*hashtable).keylist[hash(key)] = cria_hlist(key, (*hashtable).keylist[hash(key)]);
	return;
}

// Calcula o valor hash de uma string. Cada caracter entre 'a' e 'z' é associado a um número ímpar de 3 até 53.
// Esse número ímpar é multiplicado pela potência de 2 correspondente ao índice da string, de 2^0 até 2^24 para
// strings de até 25 caracteres. Por fim, o somatório é submetido a "mod TABLESIZE" e retornado. TABLESIZE foi
// escolhido grande o suficiente para tornar colisões muito improváveis.
int hash(char *key) {
	int hashvalue = 0;
	for (int i = 0; i < strlen(key); i++) {
		int charvalue = (key[i] - 'a' + 1) * 2 + 1;
		hashvalue += (charvalue * pow(2, i));
	}
	hashvalue %= TABLESIZE;
	return hashvalue;
}