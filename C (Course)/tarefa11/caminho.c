/******************************************************************
* MC202 E - 2S/2020 
* Aluno: Daniel Credico de Coimbra
* RA: 155077
* Data: 26 de Dezembro de 2020
* Descrição: Tarefa 11 | Pokemon Teclado & Mouse
* Na vida real, passa pela mente de todo profissional liberal
* construir um plano B. Isso não diferente com Red, um aspirante
* a treinador Pokémon de classe média que viu-se forçado por suas
* circunstâncias a graduar-se em Computação. Seu equilíbrio vida-
* trabalho foi magistralmente obtido pela jogatina eficiente de
* Pokémon Go no campus da Unicamp. O algoritmo abaixo minimiza a
* distância que Red precisa viajar sem passar por um PokéStop, até
* que ele possa chegar em alguma raid com o Pokémon Lugia.
*******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "grafo.h"

int has_path(grafo *gr, int path_length);
int has_path_rec(grafo *gr, int row, int path_length, int *visited);

int main(void) {
	// Inicializa o grafo e insere Red em sua posição.
	grafo *gr = cria_grafo();
	double x, y;
	scanf("%lf %lf", &x, &y);
	insere_nodulo(gr, x, y, "Red");

	// Recebe cada um dos PokéStops e raids Lugia e os insere no grafo em suas respectivas posições.
	// As distâncias entre cada grafo são calculadas e armazenadas em uma matriz adjacência.
	char *tipo = malloc(9 * sizeof(char));
	memset(tipo, '\0', 9 * sizeof(char));
	while (scanf("%lf %lf %s", &x, &y, tipo) != EOF)
		insere_nodulo(gr, x, y, tipo);
	free(tipo);

	// Para cada valor de 'path_length', busca via has_path() se existe uma rota de Red até uma raid Lugia
	// que não passe por uma aresta maior do que 'path_length', com objetivo de buscar o menor tal valor.
	// Para evitar ineficiência em casos que o menor tal valor é muito grande, 'path_length' inicia em 100
	// e é acrescido de mais 100 a cada iteração, até que se encontre caminho. Ao encontrar o primeiro caminho,
	// os 99 dígitos entre (path_length) e (path_length - 100), encontrando então o verdadeiro menor.
	int path_length = 0;
	int proceed = 1;
	while (proceed) {
		if (has_path(gr, path_length)) {
			// Se há caminho para path_length múltiplo de 100, busca-se os 99 dígitos anteriores pelo real menor caminho.
			for (int backtrack = path_length - 99; backtrack < path_length; backtrack++) {	
				if (has_path(gr, backtrack)) {
					// O real menor caminho por encontrado.
					path_length = backtrack;
					break;
				}
			}
			proceed = 0;
		}
		if (proceed)
			path_length += 100;
	}

	printf("%d\n", path_length);
	free_grafo(gr);

	return 0;
}

// Invocando has_path_rec(), realiza buscas de profundidade primeiro (DSF, depth-first search) sem atravessar arestas maiores do que 'path_length'.
int has_path(grafo *gr, int path_length) {
	int *visited = malloc((*gr).size * sizeof(int));
	memset(visited, 0, (*gr).size * sizeof(int));
	int result = has_path_rec(gr, 0, path_length, visited);
	free(visited);
	return result;
}

// A DFS é realizada recursivamente sem revisitar nódulos.
int has_path_rec(grafo *gr, int row, int path_length, int *visited) {
	visited[row] = 1;
	for (int col = 0; col < (*gr).size; col++) {
		// Se o nódulo novo ('col') ainda não foi visitado e a aresta do nódulo atual ('row') ao novo não é manor do que 'path_length'... 
		if (!visited[col] && (*gr).mtx_dst[row][col] <= path_length) {
			// Se Lugia foi encontrado...
			if ((*gr).mtx_pos[col][2])
				return 1;
			// Se não foi, uma nova instância recursiva é chamada, até encontrar-se um Lugia.
			else if(has_path_rec(gr, col, path_length, visited))
				return 1;
		}
	}
	// Se nenhum Lugia foi encontrado, dada a restrição dada por 'path_length'.
	return 0;
}