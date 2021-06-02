/***************************************************************
* MC202 E - 2S/2020 
* Aluno: Daniel Credico de Coimbra
* RA: 155077
* Data: 05 de Outubro de 2020
* Descrição: Tarefa 04 | Sea of Words
* Recebe uma tabela de letras, uma lista de palavras, e procura
* instâncias dessas palavras em caminhos verticais e horizontais
* dentro da tabela.
****************************************************************/

#include <stdio.h>
#include <stdlib.h>

int contem_palavra(char **texto, char *word, int rows, int cols);
int seek(char **texto, int i, int j, int rows, int cols, int **prev_chars, char *word, int char_atual);
int unused_letter(int **prev_chars, int i, int j);

int main(void) {
	/* Obtém os valores n, m, e q no enunciado. */
	int rows, cols, words;
	scanf("%d %d %d", &rows, &cols, &words);

	/* Obtém a tabela de letras com alocação dinâmica. */
	char **texto = malloc(100 * sizeof(char *));
	for (int row = 0; row < 100; row++)
		texto[row] = malloc(100 * sizeof(char));

	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++)
			scanf(" %c", &texto[row][col]);
	}
	scanf("\n");

	/* Obtém a lista de palavras com alocação dinâmica. */
	char **palavras = malloc(10 * sizeof(char *));
	for (int palavra = 0; palavra < 10; palavra++)
		palavras[palavra] = malloc(20 * sizeof(char));

	char c;
	int caracter = 0;
	for (int word = 0; word < words; word++) {
		while (scanf("%c", &c) != EOF && c != '\n') {
			palavras[word][caracter++] = c;
		}
		palavras[word][caracter] = '\0';
		caracter = 0; 
	}

	/* Percorre a lista de palavras determinando se estão na tabela. */
	for (int word = 0; word < words; word++) {
		if (word > 0)
			printf("\n");
		if (contem_palavra(texto, palavras[word], rows, cols))
			printf("sim");
		else
			printf("nao");
	}

	/* Libera a memória dinamicamente alocada. */
	for (int row = 0; row < 100; row ++)
		free(texto[row]);
	free(texto);

	for (int palavra = 0; palavra < 10; palavra++)
		free(palavras[palavra]);
	free(palavras);

	return 0;
}

int contem_palavra(char **texto, char *word, int rows, int cols) {
	/* Emprega a estratégia de backtracking com recursão para determinar se uma palavra
	*  está contida em uma tabela de letras.  Ao detectar um caracter de uma palavra,
	*  se registra em prev_chars qual a posição dos caracteres encontrados previamente.
	*/

	/* Cria, aloca, e inicializa o local de registro de caracteres já encontrados. */
	int **prev_chars = malloc(20 * sizeof(int *));
	for (int i = 0; i < 20; i++)
		prev_chars[i] = malloc(2 * sizeof(int));
	for (int k = 0; k < 20; k++) {
		prev_chars[k][0] = -1;
		prev_chars[k][1] = -1;
	}

	/* Percorre a tabela procurando o primeiro caracter da palavra contida em word. */
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (seek(texto, i, j, rows, cols, prev_chars, word, 0)) {
				for (int k = 0; k < 20; k++)
					free(prev_chars[k]);
				free(prev_chars);
				return 1;
			}
			else {
				// Caso não se tenha encontrado a palavra, prev_chars é esvaziado para a próxima tentativa.
				for (int k = 0; k < 20; k++) {
					prev_chars[k][0] = -1;
					prev_chars[k][1] = -1;
				}
			}
		}
	}
	
	for (int k = 0; k < 20; k++)
		free(prev_chars[k]);
	free(prev_chars);
	return 0;
}


int seek(char **texto, int i, int j, int rows, int cols, int **prev_chars, char *word, int char_atual) {
	/* Implementa o backtracking recursivo mencionado na documentação de contem_palavra(). */
	if (char_atual == 20 || word[char_atual] == '\0')
		return 1;

	/* Atualiza prev_chars a cada letra encontrada iteração. */
	if (word[char_atual] == texto[i][j]) {
		prev_chars[char_atual][0] = i;
		prev_chars[char_atual][1] = j;

		if (i > 0) {
			if (unused_letter(prev_chars, i - 1, j)) {
				if (seek(texto, i - 1, j, rows, cols, prev_chars, word, char_atual + 1))
					return 1;
			}
		}

		if (i < rows - 1) {
			if (unused_letter(prev_chars, i + 1, j)) {
				if (seek(texto, i + 1, j, rows, cols, prev_chars, word, char_atual + 1))
					return 1;
			}
		}
	
		if (j > 0) {
			if (unused_letter(prev_chars, i, j - 1)) {
				if (seek(texto, i, j - 1, rows, cols, prev_chars, word, char_atual + 1))
					return 1;
			}
		}

		if (j < cols - 1) {
			if (unused_letter(prev_chars, i, j + 1)) {
				if (seek(texto, i, j + 1, rows, cols, prev_chars, word, char_atual + 1))
					return 1;
			}
		}
	}

	/* A função chega aqui se e somente se a palavra não foi encontrado recursivamente, e prev_chars é revertido ao estado anterior. */
	prev_chars[char_atual][0] = -1;
	prev_chars[char_atual][1] = -1;
	return 0;
}


int unused_letter(int **prev_chars, int i, int j) {
	/* Determina se uma letra da tabela já foi considerada como parte da palavra sob investigação. */
	for (int k = 0; k < 20; k++) {
		if (prev_chars[k][0] == i && prev_chars[k][1] == j)
			return 0; 
	}

	return 1;
}