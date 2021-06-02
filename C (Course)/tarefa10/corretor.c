/******************************************************************
* MC202 E - 2S/2020 
* Aluno: Daniel Credico de Coimbra
* RA: 155077
* Data: 25 de Dezembro de 2020
* Descrição: Tarefa 10 | Corretor
* Um acadêmico cansado gostaria de encontrar erros de digitação
* em seu mais recente manuscrito, inclusive com a informação
* do quão grande são esses erros em relação a um dicionário.
* O programa informa se certas palavras foram digitas corretamente,
* se possuem um caracter faltando, extra, ou trocado, ou se possui
* dois ou mais erros de digitação.
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "hash.h"

int check_amarelo(htable *dict, char *key);
int char_extra(char *input, char *dictword);
int char_trocado(char *input, char *dictword);
int char_faltando(char *input, char *dictword);

int main(void) {
	// Recebe o número de palavras no dicionário e a serem verificadas.
	int size_dict, num_words;
	scanf("%d %d", &size_dict, &num_words);

	// Preenche o dicionário na forma de uma hashtable dinamicamente alocado.
	htable *dict = cria_htable();
	for (int i = 0; i < size_dict; i++) {
		char *input = malloc(KEYSIZE * sizeof(char));
		scanf("%s", input);
		insere_htable(dict, input); // insere_htable() calcula o hash de 'input' e libera a memória de 'input'.
	}

	// Recebe as palavra a serem verificadas e diz se estão corretas ('verde') ou se contêm um ('amarelo') ou mais ('vermelho') erros.
	for (int i = 0; i < num_words; i++) {
		char *input = malloc(KEYSIZE * sizeof(char));
		scanf("%s", input);
		int hashvalue = hash(input);

		// Utiliza a hashtable para averiguar se a palavra está no dicionário em tempo O(1).
		int green = 0;
		hlist *hash_ptr = (*dict).keylist[hashvalue];
		while (!green && hash_ptr != NULL) {
			if (hash((*hash_ptr).key) == hash(input))
				green = 1;
			hash_ptr = (*hash_ptr).next;
		}

		// Determina a categoria de erros da palavra em relação ao dicionário.
		if (green)
			printf("verde\n");
		else if (check_amarelo(dict, input))
			printf("amarelo\n");
		else
			printf("vermelho\n");

		free(input);
	}

	free_htable(dict);

	return 0;
}

// Compara uma palavra com o dicionário, checando se a palavra possui um ou mais erros em relação a alguma palavra do dicionário.
int check_amarelo(htable *dict, char *key) {
	hlist *hash_ptr;
	// O dicionário inteiro será percorrido.
	for (int i = 0; i < TABLESIZE; i++) {
		hash_ptr = (*dict).keylist[i];
		if (hash_ptr == NULL) // Esta é a gigantesca maioria dos casos, pois há muito menos palavras do que entradas no dicionário (TABLESIZE >> size_dict).
			continue;
		else {
			while (hash_ptr != NULL) {
				int faltando = char_faltando(key, (*hash_ptr).key);
				int extra = char_extra(key, (*hash_ptr).key);
				int trocado = char_trocado(key, (*hash_ptr).key);
				if (faltando || extra || trocado) // Se há exatamente um erro. Os três disjuntos são mutuamente excludentes. Categoria 'amarelo'.
					return 1;
				else
					hash_ptr = (*hash_ptr).next;
			}
		}
	}
	// Se a palavra não dista um erro em relação a nenhuma palavra do dicionário, então sua categoria será 'vermelho'.
	return 0;
}

// 'input' é idêntico a 'dictword' a menos de um caracter sse 'dictword' é idêntico a 'input' mas com um caracter extra.
int char_faltando(char *input, char *dictword) {
	return char_extra(dictword, input);
}

// Verifica se 'input' é idêntico a 'dictword' mas com um caracter extra.
int char_extra(char *input, char *dictword) {
	if (strlen(input) != strlen(dictword) + 1)
		return 0;
	else {
		// Percorre o comprimento de input tentando encaixar segmentos de 'dictword' em 'input'.
		// 'dictword' é pareado letra-a-letra com 'input' até que se consiga fazer esse encaixe.
		// Se um despareamento foi encontrado e não houve encaixe, então 'dictword' não possui a similaridade almejada.       
		for (int i = 0; i < strlen(input) - 1; i++) {
			int L = strlen(dictword) - i;
			// Checa se 'dictword' está contido em 'input' entre os índices i e i+L.
			if (strncmp(&input[i], &dictword[i], L) == 0)
				return 1;
			// Checa se 'dictword' está contido em 'input' entre os índices i+1 e i+1+L.
			else if (strncmp(&input[i+1], &dictword[i], L) == 0)
				return 1;
			// Não havendo nenhum dos dois encaixes, 'dictword' e 'input' precisariam parear no índice i.
			else if (input[i] != dictword[i])
				return 0;
			else
				continue;
		}
	}
	return 0;
}

// Verifica se 'input' é idêntico a 'dictword' a menos de um caracter diferente.
int char_trocado(char *input, char *dictword) {
	if (strlen(input) != strlen(dictword)) // Precisam ter o mesmo comprimento.
		return 0;

	// Conta quantos caracteres diferentes os dois possuem.
	int erros = 0;
	for (int i = 0; i < strlen(input); i++) {
		if (input[i] != dictword[i])
			erros++;
	}

	if (erros == 1)
		return 1;
	else
		return 0;
}