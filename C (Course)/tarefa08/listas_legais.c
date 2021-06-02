/***************************************************************
* MC202 E - 2S/2020 
* Aluno: Daniel Credico de Coimbra
* RA: 155077
* Data: 19 de Novembro
* Descrição: Tarefa 08 | Lisas Legais
* Implementa as operações de uma árvore rubro-negra esquerdista,
* alocada dinamicamente, e a utiliza para armazenar números de
* tamanho até 10^10. Determina quantas remoções de nódulos seria
* necessária para tornar a árvore numa "lista legal".
****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "balanced_binary_tree.h"

BBT *get_nums(BBT *tree, int num_nums);

int main(void) {
	// Preenche a árvore rubro-negra esquerdista de números.
	BBT *tree = set_BBT(0, NULL, NULL, black);
	int num_nums, num_ops;
	scanf("%d", &num_nums);
	scanf("%d", &num_ops);
	tree = get_nums(tree, num_nums);

	// Captura cada requisição de opeação e chama a função apropriada para realizá-la, imprimindo o resultado se for o caso.
	int input_op;
	for (int op = 0; op < num_ops; op++) {
		scanf("%d", &input_op);
		if (input_op == 1) {
			// Insere um número designado na árvore.
			int input_num;
			scanf("%d", &input_num);
			tree = inserir_BBT(tree, input_num);
		} else if (input_op == 2) {
			// Imprime na tela a quantidade de ocorrências ("reps") de um número na árvore.
			int input_num;
			scanf("%d", &input_num);
			printf("%d\n", count_BBT(tree, input_num));
		} else
			// Imprime na tela quantos números precisam ser removidos para transformar a árvore em uma "lista legal".
			printf("%d\n", legalize_BBT(tree));
	}

	// Libera a memória dinamicamente alocada para armazenar a árvore rubro-negra esquerdista.
	free_BBT(tree);

	return 0;
}

BBT *get_nums(BBT *tree, int num_nums) {
	// Captura iterativamente os números e os insere na árvore rubro-negra esquerdista.
	int input_num;
	scanf("%d", &input_num);
	(*tree).val = input_num;
	for (int num = 1; num < num_nums; num++) {
		scanf("%d", &input_num);
		tree = inserir_BBT(tree, input_num);
	}
	return tree;
}