#include <stdio.h>
#include <stdlib.h>
#include "balanced_binary_tree.h"

BBT *set_BBT(int val, BBT *esq, BBT *dir, enum Cor cor) {
	// Aloca dinamicamente uma árvore rubro-negra esquerdista com valores informados nos parâmetros.
	BBT *new = malloc(sizeof(BBT));
	(*new).val = val;
	(*new).reps = 1;
	(*new).esq = esq;
	(*new).dir = dir;
	(*new).cor = cor;
	return new;
}

BBT *get_BBT(BBT *tree, int val) {
	// Percore a aŕvore rubro-negra esquerdista iterativamente buscando um nódulo com o valor "val". Eficiência O(lg n).
	if (tree == NULL || (*tree).val == val)
		return tree;
	if (val < (*tree).val)
		return get_BBT((*tree).esq, val);
	else
		return get_BBT((*tree).dir, val);
}

void free_BBT(BBT *tree) {
	// Libera recursivamente a memória dinamicamente alocada para armazenar uma árvore rubro-negra esquerdista.
	if (tree == NULL)
		return;
	else {
		if ((*tree).esq != NULL)
			free_BBT((*tree).esq);
		if ((*tree).dir != NULL)
			free_BBT((*tree).dir);
		free(tree);
	}
	return;
}

BBT *inserir_BBT(BBT *tree, int val) {
	// Caso se tente inserir um número que já está na árvore, ele não é inserido. Ao invés disso, sua propriedade "reps" (no. de ocorrências) é acrescida de um.
	if (count_BBT(tree, val) == 0) {
		// Insere recursivamente um número novo na árvore.
		tree = inserir_BBT_rec(tree, val);
		(*tree).cor = black;
	} else {
		BBT *target = get_BBT(tree, val);
		(*target).reps++;
	}
	return tree;
}

BBT *inserir_BBT_rec(BBT *tree, int val) {
	// Insere recursivamente um número novo numa árvore rubro-negra esquerdista. Eficiência: O(lg n).

	// Se chegamos em uma folha, inserimos um novo nódulo vermelho com o valor designado.
	if (tree == NULL)
		return set_BBT(val, NULL, NULL, red);
	
	// Se não estamos em uma folha, faremos uma nova chamada recursiva e corrigiremos seus efeitos para re-obter uma árvore rubro-negra esquerdista.
	else {
		if (val <= (*tree).val)
			(*tree).esq = inserir_BBT_rec((*tree).esq, val);
		else
			(*tree).dir = inserir_BBT_rec((*tree).dir, val);

		// Dois nós esquerdos vermelhos consecutivos se tornarão um nó vermelho à esquerda e outro à direita.
		if (isred_BBT((*tree).esq) && isred_BBT((*tree).esq->esq))
			tree = mirror_right_BBT(tree);
	
		if (isred_BBT((*tree).dir)) {
			// Corrige situação com um nó vermelho à esquerda e outro à direita.
			if (isred_BBT((*tree).esq))
				raise_color_BBT(tree);
			// Corrige situação apenas com um nó vermelho à direita.
			else
				tree = mirror_left_BBT(tree);
		}

		return tree;
	}

}

BBT *mirror_left_BBT(BBT *tree) {
	// Espelha uma árvore rubro-negra esquerdista para a esquerda. Função empregada em sua correção em inserir_BBT_rec().
	BBT *newroot = ((*tree).dir);
	(*tree).dir = (*newroot).esq;
	(*newroot).esq = tree;
	(*newroot).cor = (*tree).cor;
	(*tree).cor = red;
	return newroot;
}


BBT *mirror_right_BBT(BBT *tree) {
	// Espelha uma árvore rubro-negra esquerdista para a direita. Função empregada em sua correção em inserir_BBT_rec().
	BBT *newroot = (*tree).esq;
	(*tree).esq = (*newroot).dir;
	(*newroot).dir = tree;
	(*newroot).cor = (*tree).cor;
	(*tree).cor = red;
	return newroot;
}

void raise_color_BBT(BBT *tree) {
	// Se um nódulo possui dois filhos vermelhos, ele se torna vermelho e seus filhos pretos. Função empregada em sua correção em inserir_BBT_rec().
	if (tree != NULL) {
		(*tree).cor = red;
		if ((*tree).esq != NULL)
			(*tree).esq->cor = black;
		if ((*tree).dir != NULL)
			(*tree).dir->cor = black;
	}
	return;
}

int count_BBT(BBT *tree, int val) {
	// Retorna o número de vezes em que um número ocorre numa árvore rubro-negra esquerdista.
	BBT *target = get_BBT(tree, val);
	if (target != NULL)
		return (*target).reps;
	else
		return 0;
}

int legalize_BBT(BBT *tree) {
	// Conta recursivamente quantos números precisam ser removidos de uma aŕvore rubro-negra esquerdista para torná-la legal ("legalizá-la").
	if (tree == NULL)
		return 0;
	else {
		// Para cada número na árvore, determina quantas vezes ele ocorre em falta ou em excesso.
		int total = (*tree).reps;
		if (total >= (*tree).val) // Exemplo: Se '3' ocorre 2 vezes, total é 2. Se '3' ocorre 3 vezes, total é (3 - 3) = 0. Se '3' ocorre 4 vezes, total é (4 - 3) = 1.
			total -= (*tree).val;
		total += legalize_BBT((*tree).esq);
		total += legalize_BBT((*tree).dir);
		return total;
	}
}

int isred_BBT(BBT *tree) {
	// Determina se um nódulo é vermelho.
	if (tree == NULL)
		return 0;
	else {
		if ((*tree).cor == red)
			return 1;
		else
			return 0;
	}
}

int isblack_BBT(BBT *tree) {
	// Determina se um nódulo é preto.
	if (tree == NULL)
		return 1;
	else {
		if ((*tree).cor == black)
			return 1;
		else
			return 0;
	}
}