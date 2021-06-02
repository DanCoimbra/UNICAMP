/****************************************************************
* MC202 E - 2S/2020 
* Aluno: Daniel Credico de Coimbra
* RA: 155077
* Data: 27 de Novembro de 2020
* Descrição: Tarefa 07 | O Mensageiro
* Um mesangeiro possui pedaços de mensagens embaralhadas em
* árvores binárias de busca, e precisa encontrar uma sequência
* de indivíduos que o ajudarão a desembralhá-las. As mensagens
* desembaralhadas são impressas na tela conforme são descobertas.
*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binary_tree.h"

int main(void) {
	int num_tokens, num_auths, id_token, id_aut;
	BT *saco = NULL;
	BT *triade = NULL;
	TK *token = NULL;

	while (scanf("%d %d", &num_tokens, &num_auths) != EOF) {
		// Preenche o saco com os tokens pertinente ao grupo atual de autoridades.
		for (int i = 0; i < num_tokens; i++) {
			char *txt = malloc(6 * sizeof(char));
			scanf("%d \"%[^\"]\"", &id_token, txt);
			token = set_TK(id_token, txt);
			saco = inserir_BT(saco, token);
		}

		// Percorre o grupo atual de autoridades sintetizando as tríades apropriadas de cartões.
		for (int j = 0; j < num_auths; j++) {	
			scanf("%d", &id_aut);
			// Obtém uma árvore binária de busca contendo os três tokens cuja soma iguala o ID da autoridade.
			triade = get_triade_BT(saco, id_aut);
			// Obtém um token cuja mensagem é a concatenação das mensagems dos três tokens em ordem crescente.
			token = merge_triade(triade, id_aut);
			// A variável 'flag' armazena se a variável 'saco' foi completamente liberada em remover_triade_BT().
			int flag = remover_triade_BT(saco, triade);
			if (flag)
				saco = NULL;
			// Insere no saco o novo token, obtido a partir da síntese da tríade acima.
			saco = inserir_BT(saco, token);
		}
		// Imprime na tela a concatenação das mensagens dos tokens no saco em ordem crescente.
		print_msg(saco); // Também libera 'saco'.
		saco = NULL;
	}

	return 0;
}