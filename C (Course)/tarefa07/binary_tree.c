#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binary_tree.h"

// Inicializa um token com alocação dinâmica de memória.
TK *set_TK(int id, char *msg) {
	TK *token = malloc(sizeof(TK));
	(*token).id = id;
	(*token).msg = msg;
	return token;
}

// Copia um token, alocando dinamicante a cópia em outra região da memória.
TK *copy_TK(TK *original) {
	int new_val = (*original).id;
	char *new_msg = malloc((strlen((*original).msg) + 1) * sizeof(char));
	strcpy(new_msg, (*original).msg);
	TK *copia = set_TK(new_val, new_msg);
	return copia;
}

// Inicializa uma árvore binária de busca com alocação dinâmica de memória.
BT *set_BT(TK *token, BT *esq, BT *dir, BT *pai) {
	BT *new = malloc(sizeof(BT));

	(*new).token = token;
	(*new).esq = esq;
	(*new).dir = dir;
	(*new).pai = pai;

	// Atualiza os filhos para colocar a si mesmo como pai.
	if (esq != NULL)
		(*esq).pai = new;
	if (dir != NULL)
		(*dir).pai = new;

	return new;
}

// Busca na árvore binária de busca o token cujo ID único é igual ao parâmetro 'id'.
BT *get_BT(BT *tree, int id) {
	if (tree == NULL || (*tree).token->id == id)
		return tree;
	if (id < (*tree).token->id)
		return get_BT((*tree).esq, id);
	else
		return get_BT((*tree).dir, id);
}

// Libera recurisvamente a memória dinamicamente alocada numa árvore binária de busca.
void free_BT(BT *tree) {
	if (tree != NULL) {
		// Recurisvamente libera os filhos.
		if ((*tree).esq != NULL)
			free_BT((*tree).esq);
		if ((*tree).dir != NULL)
			free_BT((*tree).dir);

		// Libera o token.
		free((*tree).token->msg);
		free((*tree).token);

		free(tree);
	}
	return;
}

// Insere um token na árvore binária de busca (ABB), mantendo sua propriedade como ABB. Cria árvore se 'tree' for NULL.
BT *inserir_BT(BT *tree, TK *token) {
	if (tree == NULL)
		return set_BT(token, NULL, NULL, NULL);
	else {
		if ((*token).id < (*tree).token->id) {
			// Insere à esquerda. Se houver sub-árvore esquerda, faz chamada recursiva. Senão, cria novo nódulo.
			if ((*tree).esq == NULL)
				(*tree).esq = set_BT(token, NULL, NULL, tree);
			else
				(*tree).esq = inserir_BT((*tree).esq, token);
		} else {
			// Insere à direita. Se houver sub-árvore direita, faz chamada recursiva. Senão, cria novo nódulo.
			if ((*tree).dir == NULL)
				(*tree).dir = set_BT(token, NULL, NULL, tree);
			else
				(*tree).dir = inserir_BT((*tree).dir, token);
		}
		return tree;
	}
}

// A partir do ID único, remove um token de uma árvore binária de busca (ABB), mantendo sua propriedade como ABB.
void remover_BT(BT *tree, int id) {
	BT *alvo = get_BT(tree, id);
	if (alvo == NULL)
		// Se não há tal alvo (i.e. um token com tal ID), nada é feito.
		return;

	// Se o alvo não possui filhos, ele é removido prontamente.
	if ((*alvo).esq == NULL && (*alvo).dir == NULL) {
		if ((*alvo).pai != NULL) {
			// Destrói a ligação do pai com o filho, dependendo se a ligação é à esquerda ou à direita.
			if (cmp_BT(alvo, (*alvo).pai->esq))
				(*alvo).pai->esq = NULL;
			else
				(*alvo).pai->dir = NULL;
		}
		free_BT(alvo);

	// Já se o alvo possui filhos...
	} else {
		BT *sub;
		if ((*alvo).esq != NULL)
			sub = prev_BT(alvo); // Se o alvo possui sub-árvore esquerda, ele é substituído pelo seu antecessor.
		else
			sub = next_BT(alvo); //Se o alvo possui sub-árvore direita, ele é substituído pelo seu sucessor.

		int new_val = (*sub).token->id;
		char *new_msg = malloc((strlen((*sub).token->msg) + 1) * sizeof(char));
		strcpy(new_msg, (*sub).token->msg);
		remover_BT(tree, new_val);
		(*alvo).token->id = new_val;
		free((*alvo).token->msg);
		(*alvo).token->msg = new_msg;
	}
}

// Dado um nódulo na árvore, encontra o token cujo ID único antecede o ID do nódulo. Por exemplo, se não houver nódulo com ID = 6, então o ID = 5 antece o ID = 7.
BT *prev_BT(BT *tree) {
	if (tree == NULL)
		return NULL;

	// Se não há sub-árvore esquerda, o antecessor não estará abaixo na árvore.
	else if ((*tree).esq == NULL) {
		BT *uptree = (*tree).pai;

		loop: ; // Ponto de retorno caso o pai seja ancestral à direita.

		// Se não há sub-árvore esquerda nem pai, não há antecessor.
		if (uptree == NULL) {
			return NULL;
		}

		// Se o pai é ancestral à esquerda — isto é, se o nódulo é filho direito de seu pai —, então o próprio pai é o antecessor.
		else if (cmp_BT(tree, (*uptree).dir))
			return uptree;

		// Se o pai é ancestral à direita, a investigação continua até encontrar o primeiro ancestral à esquerda.
		else {
			tree = uptree;
			uptree = (*uptree).pai;
			goto loop;
		}

	// Se há sub-árvore esquerda, o antecessor nela estará.
	} else {

		// Se o filho esquerdo não possui sub-árvore direita, tal filho é o antecessor.
		if ((*tree).esq->dir == NULL)
			return (*tree).esq;

		// Se o filho direito possui sub-árvore direita, o antecessor é o máximo dessa sub-árvore.
		else
			return max_BT((*tree).esq->dir);
	}
}

// Dado um nódulo na árvore, encontra o token cujo ID único sucede o ID do nódulo. Por exemplo, se não houver nódulo com ID = 6, então o ID = 7 sucede o ID = 5.
BT *next_BT(BT *tree) {
	if (tree == NULL)
		return NULL;

	// Se não há sub-árvore direita, o sucessor não estará abaixo na árvore.
	else if ((*tree).dir == NULL) {
		BT *uptree = (*tree).pai;

		loop: ; // Ponto de retorno caso o pai seja ancestral à esquerda.

		// Se não há sub-árvore direita nem pai, não há sucessor.
		if (uptree == NULL)
			return NULL;

		// Se o pai é ancestral à direita — isto é, se o nódulo é filho esquerdo de seu pai —, então o pai é o sucessor.
		else if (cmp_BT(tree, (*uptree).esq))
			return uptree;

		// Se o pai é ancestral à esquerda, a investigação continua até encontrar o primeiro ancestral à direita.
		else {
			tree = uptree;
			uptree = (*uptree).pai;
			goto loop;
		}

	// Se há sub-árvore direita, o sucessor nela estará.
	} else {
		// Se o filho direito não possui sub-árvore esquerda, tal filho é o sucessor.
		if ((*tree).dir->esq == NULL)
			return (*tree).dir;

		// Se o filho direito possui sub-árvore esquerda, o sucessor é o mínimo dessa sub-árvore.
		else
			return min_BT((*tree).dir->esq);
	}
}

// Encontra o nódulo com maior ID da árvore. Será o nódulo mais à direita.
BT *max_BT(BT *tree) {
	if ((*tree).dir == NULL)
		return tree;
	else
		return max_BT((*tree).dir);
}

// Encontra o nódulo com menor ID da árvore. Será o nódulo mais à esquerda.
BT *min_BT(BT *tree) {
	if ((*tree).esq == NULL)
		return tree;
	else
		return min_BT((*tree).esq);
}

// Dado que os tokens possuem IDs únicos e cada token está em apenas um nódulo, pode-se determinar se dois nódulos são idênticos comparando os IDs de seus tokens.
int cmp_BT(BT *tree1, BT *tree2) {
	// Se uma árvore é NULL e a outra não, não são idênticas.
	if (tree1 == NULL) {
		if (tree2 == NULL)
			return 1;
		else
			return 0;
	} else if (tree2 == NULL)
		return 0;

	// 
	if ((*tree1).token->id != (*tree2).token->id)
		return 0;

	return 1;
}

// Determina três tokens cujos IDs únicos somados igualam ao ID da autoridade.
BT *get_triade_BT(BT *saco, int id_aut) {
	// A busca começa pelos três menores tokens.
	BT *min = min_BT(saco);
	BT *med = next_BT(min);
	BT *max = next_BT(med);

	// Exemplo: Se nossa árvore possui os tokens 1, 2, 3, 4, 5, 6 nossa busca passa pela seguinte sequência.
	// 1, 2, 3 >>> 1, 2, 4 >>> 1, 2, 5 >> 1, 2, 6
	// 1, 3, 4 >>> 1, 3, 5 >>> 1, 3, 6
	// 1, 4, 5 >>> 1, 4, 6
	// 1, 5, 6
	// 2, 3, 4 >>> 2, 3, 5 >> 2, 3, 6
	// 2, 4, 5 >>> 2, 4, 6
	// 2, 5, 6
	// 3, 4, 5 >>> 3, 4, 6
	// 4, 5, 6
	int sum;
	while (min != NULL && (*min).token->id < id_aut/3) {
		// 'min' percorre a árvore em ordem crescente até chegar em um terço de id_aut. (Sendo 'med' e 'max' maiores, a soma não bate com 'min' maior do que isso.)
		
		while (med != NULL && (*med).token->id < id_aut/2) {
		// Mantendo 'min' fixo, 'med' começa como sucessor de 'min' e percorre a árvore em ordem crescente até chegar na metade de id_aut.
		// (Como 'min' é não-nulo e 'max' é maior que 'med', não haveria como a soma bater enquanto 'med' fosse maior que a metade de id_aut.)
		
			while (max != NULL) {
				// Mantendo 'min' e 'med' fixos, 'max' começa como sucessor de 'med' e percorre a árvore em ordem crescente até acabar.
		
				sum = (*min).token->id + (*med).token->id + (*max).token->id;
				if (sum == id_aut)
					goto endloop; // Assim que a soma correta é encontrada, saímos dos três loops aninhados.
				else if (sum > id_aut)
					break; // Se a soma ficou do que id_aut, não adianta prosseguir aumentando 'max'.
		
				max = next_BT(max);
			}
		
			med = next_BT(med);
			max = next_BT(med);
		
		}
		
		min = next_BT(min);
		med = next_BT(min);
		max = next_BT(med);
	
	}

	endloop: ;

	// Para posteriormente poder eliminar do saco os componentes da tríade sem destruir a própria tríade, os componentes são copiados dos originais no saco.
	BT *triade = NULL;
	TK *min_copy = copy_TK((*min).token);
	TK *med_copy = copy_TK((*med).token);
	TK *max_copy = copy_TK((*max).token);
	triade = inserir_BT(triade, min_copy);
	triade = inserir_BT(triade, med_copy);
	triade = inserir_BT(triade, max_copy);

	return triade;
}

// Concatena as mensagens dos tokens da tríade. A tríade é sempre uma árvore binária de busca com três elementos, sem nenhuma sub-árvore esquerda.
TK *merge_triade(BT *triade, int id_aut) {
	// Captura o tamanho que resultaria as strings concatenadas. A soma começa em 1 para dar espaço para '\0'.
	int tamanho = 1;
	tamanho += strlen((*triade).token->msg);
	tamanho += strlen((*triade).dir->token->msg);
	tamanho += strlen((*triade).dir->dir->token->msg);

	char *new_msg = malloc(tamanho * sizeof(char));
	*new_msg = '\0';

	// Concaneta as mensagens em ordem crescente.
	strcat(new_msg, (*triade).token->msg);
	strcat(new_msg, (*triade).dir->token->msg);
	strcat(new_msg, (*triade).dir->dir->token->msg);

	return set_TK(id_aut, new_msg);
}

// Remove do saco os componentes da tríade. Libera a memória dinamicamene alocada para a tríade.
int remover_triade_BT(BT *saco, BT *triade) {
	remover_BT(saco, (*triade).dir->dir->token->id);
	free((*triade).dir->dir->token->msg);
	free((*triade).dir->dir->token);
	free((*triade).dir->dir);

	remover_BT(saco, (*triade).dir->token->id);
	free((*triade).dir->token->msg);
	free((*triade).dir->token);
	free((*triade).dir);

	// Se o saco for resultar vazio após remover o último componente da tríade, a função retornará um aviso de que 'saco' foi liberado.
	int flag = 0;
	if ((*saco).esq == NULL && (*saco).dir == NULL)
		flag = 1;

	remover_BT(saco, (*triade).token->id);
	free((*triade).token->msg);
	free((*triade).token);
	free(triade);

	return flag;
}

// Imprime na tela as mensagens em ordem crescente do saco. No processo, libera o saco.
void print_msg(BT *saco) {
	BT *menor = min_BT(saco);
	while (menor != NULL) {
		printf("%s", (*menor).token->msg);
		int flag = 0;
		if ((*saco).esq == NULL && (*saco).dir == NULL)
			flag = 1;
		remover_BT(saco, (*menor).token->id);
		if (flag)
			menor = NULL;
		else
			menor = min_BT(saco);
	}
	printf("\n");
	return;
}