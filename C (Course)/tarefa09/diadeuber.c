/****************************************************************
* MC202 E - 2S/2020 
* Aluno: Daniel Credico de Coimbra
* RA: 155077
* Data: 09 de Dezembro de 2020
* Descrição: Tarefa 09 | Dia de Uber
* Um trabalhador precarizado tenta ganhar dinheiro fazendo um
* dia de caronas pelo aplicativo Uber. O programa implementa
* os mecanismos de aceitar corrida, finalizar corrida, cancelar
* corrida, e finalizar o dia. Ao finalizar o dia, um relatório
* contábil do trabalho realizado é impresso na tela.
*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "heap.h"

pessoa *get_pessoa(void);
int get_distancia(int *origem, int *destino);
void print_resultados(int km_vazio, int km_cheio, int cancelamentos);

int main(void) {
	int km_cheio = 0; // Quilometragem com passageiro (lucrativo).
	int km_vazio = 0; // Quilometragem sem passageiro (só gasta gasolina).
	int cancelamentos = 0;
	int *posicao_atual = malloc(2 * sizeof(int)); // Posição no plano cartesiano 2D.
	if (posicao_atual == NULL)
		exit(1);

	posicao_atual[0] = 0;
	posicao_atual[1] = 0;

	heap *fila = cria_heap();
	
	char action;
	while (scanf("%c", &action) != EOF) {

		// Aceitar: Coleta os dados do passageito de stdin e o insere na fila de espera.
		if (action == 'A') {
			pessoa *passageiro = get_pessoa();
			fila = insere_heap(fila, passageiro);
			printf("Cliente %s foi adicionado(a)\n", (*passageiro).nome);
			if ((*fila).atual == NULL)
				(*fila).atual = passageiro; // Se este for o primeiro passageiro, ele se torna o passageiro atual (sob carona).
		}

		// Finalizar: Computa a quilometragem da carona e remove o passageiro atual da fila de espera. Coleta o próximo passageiro: aquele com maior nota na fila de espera.
		else if (action == 'F') {
			km_vazio += get_distancia(posicao_atual, (*fila).atual->origem);
			km_cheio += get_distancia((*fila).atual->origem, (*fila).atual->destino);
			// Atualiza a posição atual.
			posicao_atual[0] = (*fila).atual->destino[0];
			posicao_atual[1] = (*fila).atual->destino[1];
			printf("A corrida de %s foi finalizada\n", (*fila).atual->nome);
			fila = remove_heap(fila, (*fila).atual->nome);
			(*fila).atual = NULL;
			if ((*fila).qtd != 0)
				(*fila).atual = (*fila).itens[1]; // Coleta o próximo passageiro, se houver.
			scanf(" "); // Lê um newline ('\n') remanescente, se houver.
		}

		// Cancelar: Remove da fila de espera alguém com o nome especificado.
		else if (action == 'C') {
			char *nome = malloc(16 * sizeof(char));
			if (nome == NULL)
				exit(1);
			scanf("%s", nome);
			printf("%s cancelou a corrida\n", nome);
			fila = remove_heap(fila, nome);
			free(nome);
			cancelamentos++;
			scanf(" "); // Lê um newline ('\n') remanescente, se houver.
		}

		// Terminar (action == 'T'): termina o dia de trabalho e sai do loop.
		else
			break;
	}

	print_resultados(km_vazio, km_cheio, cancelamentos); // Imprime relatório contábil.

	free(posicao_atual);
	free_heap(fila);

	return 0;
}

// Computa a distância de Manhattan, dadas duas duplas ordenadas.
int get_distancia(int *origem, int *destino) {
	int Dx = origem[0] - destino[0];
	int Dy = origem[1] - destino[1];
	if (Dx < 0)
		Dx = -Dx;
	if (Dy < 0)
		Dy = -Dy;
	return Dx + Dy;
}

// Imprime na tela o relatório contábil do dia de trabalho.
void print_resultados(int km_vazio, int km_cheio, int cancelamentos) {
	printf("\nJornada finalizada. Aqui esta o seu rendimento de hoje\n");
	printf("Km total: %d\n", km_vazio + km_cheio);
	// O rendimento bruto é R$1,40 por quilometragem com passageiro, mais R$7,00 por cancelamento.
	double bruto = 1.4 * (double) km_cheio + 7.0 * (double) cancelamentos;
	printf("Rendimento bruto: %.2lf\n", bruto);
	// A despesa é R$57,00 fixos (aluguel do carro) mais R$0,4104 de gasolina por quilometragem qualquer.
	double despesa = 57.00 + 0.4104 * (double) (km_vazio + km_cheio);
	printf("Despesas: %.2lf\n", despesa);
	// Após uma taxa que consome 25% dos rendimentos brutos, o rendimento líquido é a diferença do rendimento bruto e das despesas.
	printf("Rendimento liquido: %.2lf\n", 0.75 * bruto - despesa);
	return;
}