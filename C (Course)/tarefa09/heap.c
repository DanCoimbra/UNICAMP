#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"

// Libera a memória dinamicamente alocada para uma pessoa.
void free_pessoa(pessoa *passageiro) {
	free((*passageiro).nome);
	free((*passageiro).origem);
	free((*passageiro).destino);
	free(passageiro);
	return;
}

// Libera a memória dinamicamente alocada para um heap (de tipo max).
void free_heap(heap *fila) {
	free((*fila).atual);
	free((*fila).itens);
	free(fila);
}

// Aloca dinamicamente memória para uma pessoa e coleta seus dados de stdin.
pessoa *get_pessoa(void) {
	pessoa *passageiro = malloc(sizeof(pessoa));
	if (passageiro == NULL)
		exit(1);
	
	char *nome = malloc(16 * sizeof(char));
	if (nome == NULL)
		exit(1);
	scanf("%s", nome);
	(*passageiro).nome = nome;

	double nota;
	scanf("%lf", &nota);
	(*passageiro).nota = nota;

	// Posições no plano cartesiano 2D são armazenadas como duplas ordenadas (vetores de duas posições).
	int x, y, z, w;
	scanf("%d %d %d %d ", &x, &y, &z, &w);
	int *origem = malloc(2 * sizeof(int));
	int *destino = malloc(2 * sizeof(int));
	if (origem == NULL || destino == NULL)
		exit(1);
	origem[0] = x;
	origem[1] = y;
	destino[0] = z;
	destino[1] = w;
	(*passageiro).origem = origem;
	(*passageiro).destino = destino;

	return passageiro;
}

// Aloca dinamicamente memória para um heap (de tipo max).
heap *cria_heap() {
	heap *fila = malloc(sizeof(heap));
	if (fila == NULL)
		exit(1);
	(*fila).atual = NULL;
	// Como há 500 linhas no máximo em stdin, e toda corrida precisa ser finalizada, encontrar-se-á no máximo 250 pessoas.
	// O vetor 'itens' possui 251 espaços porque o índice '0' não é utilizado, para facilitar as contas.
	(*fila).itens = malloc(251 * sizeof(pessoa *));
	if ((*fila).itens == NULL)
		exit(1);
	(*fila).qtd = 0;
	return fila;	
}

// Insere uma pessoa em um heap max.
heap *insere_heap(heap *fila, pessoa *passageiro) {
	// Se a fila não foi incializada ainda, isso é feito.
	if (fila == NULL) {
		fila = cria_heap();
		return insere_heap(fila, passageiro);

	// Já se a lista já foi inicializada...
	} else {
		(*fila).qtd += 1;
		(*fila).itens[(*fila).qtd] = passageiro;
		if ((*fila).qtd == 1) // Se após a adição há apenas um membro, não há o que consertar no heap.
			return fila;
		else
			return sobe_heap(fila, (*fila).qtd); // Conserta o heap, subindo a pessoa atual até que ela seja menor que seu pai.
	}
}

// Conserta um heap max após uma adição, subindo a última pessoa até que ela seja menor que seu pai.
heap *sobe_heap(heap *fila, int index_alvo) {
	if (index_alvo <= 1)
		return fila; // Se a pessoa-alvo já está no topo, não há mais o que subir.
	else {
		int index_pai = index_alvo / 2;
		pessoa *alvo = (*fila).itens[index_alvo];
		pessoa *pai = (*fila).itens[index_pai];
		if ((*alvo).nota > (*pai).nota) {
			// Sobe o filho apenas se sua nota foi maior que a do pai. Pai e filho trocam de posição no heap.
			(*fila).itens[index_alvo] = pai;
			(*fila).itens[index_pai] = alvo;
			return sobe_heap(fila, index_pai); // Nova instância recursiva continua subindo o filho.
		} else
			return fila; // Se a nota do filho é menor que a do pai, então o filho não subirá mais.
	}
}

// Remove uma pessoa de um heap max, baseado em um nome dado. Também libera a memória dinamicamente alocada para esta pessoa.
heap *remove_heap(heap *fila, char *nome) {
	// Como um heap max não é uma árvore binária de busca, ele não contém uma organização que auxilie a busca. Procuramos o nome percorrendo todo o heap.
	for (int index = 1; index <= (*fila).qtd; index++) {
		if (strcmp(nome, (*fila).itens[index]->nome) == 0) {
			free_pessoa((*fila).itens[index]);
			pessoa *ultimo = (*fila).itens[(*fila).qtd];
			(*fila).itens[index] = ultimo; // A última pessoa do heap toma lugar da pessoa-alvo, se forem diferentes.
			(*fila).qtd -= 1; // Se a pessoa-alvo for a última pessoa, esta operação efetivamente a elimina do heap.
			return desce_heap(fila, index); // Conserta o heap, descendo a pessoa substituta ("ultimo") até que ela seja maior que seus filhos.
		}
	}
	return fila;
}

// Conserta um heap max após uma removação, descendo uma dada pessoa até que ela seja maior do que seus filhos.
heap *desce_heap(heap *fila, int topo) {
	// Se o nódulo (*fila).itens[topo] não possui filhos.
	if ((*fila).qtd < 2 * topo)
		return fila;

	// Se ele possui filhos.
	else {
		pessoa *pai = (*fila).itens[topo];
		pessoa *filho_esq = (*fila).itens[2 * topo];
		
		// Este bloco testa se o filho direito existe e se é o maior dos três. Se for, troca pai e filho direito de lugar.
		if ((*fila).qtd >= 2 * topo + 1) {
			pessoa *filho_dir = (*fila).itens[2 * topo + 1];
			if ((*filho_esq).nota < (*filho_dir).nota && (*pai).nota < (*filho_dir).nota) {
				(*fila).itens[topo] = filho_dir;
				(*fila).itens[2 * topo + 1] = pai;
				return desce_heap(fila, 2 * topo + 1); // Chamada recursiva continua descendo a pessoa original.
			}
		}

		// Este bloco é executado caso o filho direito não exista ou não seja o maior dos três. Se a nota do pai é menor que a do filho esquerdo, troca os dois de lugar.
		if ((*pai).nota < (*filho_esq).nota) {
			(*fila).itens[topo] = filho_esq;
			(*fila).itens[2 * topo] = pai;
			return desce_heap(fila, 2 * topo); // Chamada recursiva continua descendo a pessoa original.
		} else {
			return fila;
		}
	}

}