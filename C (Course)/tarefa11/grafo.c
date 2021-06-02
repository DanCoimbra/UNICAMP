#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "grafo.h"

// Aloca dinamicamente a memória para um grafo. O grafo é armazenado na forma da matriz de adjacência 'mtx_dst' (matriz-distância),
// cujas entradas contém a distância em metros (inteiros, arredondados para cima) entre dois nódulos. A estrutura de dados também
//armazena as coordenadas (x,y) dos nódulos (doubles) em 'mtx_pos' (matriz-posição). Esta matriz também possui um terceiro campo
// que informa se o nódulo contém uma raid de Lugia (1) ou não (0). O tamanho das matrizes são especificados pelo macro 'MAX',
// de valor 600, grande o suficiente para armazenar na memória heap todos os grafos do exercício.
grafo *cria_grafo() {
	grafo *gr = malloc(sizeof(grafo));
	(*gr).size = 0; // Número de nódulos no grafo.

	(*gr).mtx_dst = malloc(MAX * sizeof(int *)); // Matriz de adjacência.
	// Inicializa a matriz com 0's.
	for (int i = 0; i < MAX; i++) {
		(*gr).mtx_dst[i] = malloc(MAX * sizeof(int));
		for (int j = 0; j < MAX; j++)
			(*gr).mtx_dst[i][j] = 0;
	}

	(*gr).mtx_pos = malloc(MAX * sizeof(double *)); // Coordenadas (x,y) e presença de Lugia.
	// Inicializa a matriz com 0's.
	for (int i = 0; i < MAX; i++) {
		(*gr).mtx_pos[i] = malloc(3 * sizeof(double));
		for (int j = 0; j < 3; j++)
			(*gr).mtx_pos[i][j] = 0;
	}

	return gr;
}

// Libera iterativamente a memória dinamicamente alocada para o grafo e suas duas matrizes.
void free_grafo(grafo *gr) {
	for (int i = 0; i < MAX; i++) {
		free((*gr).mtx_dst[i]);
		free((*gr).mtx_pos[i]);
	}

	free((*gr).mtx_dst);
	free((*gr).mtx_pos);

	free(gr);

	return;
}

// Insere um novo nódulo no grafo. Suas coordenadas 'x' e 'y' são inseridos em 'mtx_pos', junto
// com a informação de se seu tipo é "Lugia" ou não. Sua distância em relação a todos os outros
// nódulos anteriores também é inserida em 'mtx_dst'.
void insere_nodulo(grafo *gr, double x, double y, char *tipo) {
	// Calcula todas as distâncias: número inteiro arredondado para cima.
	for (int i = 0; i < (*gr).size; i++) {
		double dx = x - (*gr).mtx_pos[i][0];
		double dy = y - (*gr).mtx_pos[i][1];
		double dst_double = sqrt(dx * dx + dy * dy);
		int dst_int = (int) dst_double;
		double remainder = dst_double - dst_int;
		if (remainder == 0) {
			(*gr).mtx_dst[i][(*gr).size] = dst_int;
			(*gr).mtx_dst[(*gr).size][i] = dst_int;
		} else {
			(*gr).mtx_dst[i][(*gr).size] = dst_int + 1;
			(*gr).mtx_dst[(*gr).size][i] = dst_int + 1;
		}
	}

	// Armazena as coordenadas.
	(*gr).mtx_pos[(*gr).size][0] = x;
	(*gr).mtx_pos[(*gr).size][1] = y;

	// Armazena o tipo.
	if (strcmp(tipo, "Lugia") == 0)
		(*gr).mtx_pos[(*gr).size][2] = 1;

	// Atualiza o tamanho da matriz.
	(*gr).size++;
	return;
}