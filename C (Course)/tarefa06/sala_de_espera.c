/***************************************************************
* MC202 E - 2S/2020 
* Aluno: Daniel Credico de Coimbra
* RA: 155077
* Data: 21 de Novembro de 2020
* Descrição: Tarefa 06 | Sala de Espera
* Implementa salas de espera para um hospital contendo nove
* especialidades médicas. Implementa os ciclos de atendimento
* dos pacientes, para que todos possam visitar uma lista
* de especialidades anteriormente determinada. Imprime o horário
* de saída dos pacientes do hospital, acompanhado de seus nomes.
****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "deque.h"

void get_pac(pac *paciente, int id, char *c);
void primeira_chamada(dq **fila_medicos, pac *paciente);
int ciclo_de_atendimento(dq **fila_medicos, int *horario);
void mover_paciente(dq *fila_antiga, dq *fila_nova);
void print_atendimento(pac *paciente, int *horario);
dq *integrar_filas(dq *velho, dq* novo, int esp);
int cmpfunc (const void * a, const void * b);


int main(void) {
	// Captura iterativamente o input de informações dos pacientes. Variáveis "pac" se comportam como listas duplamente ligadas.
	pac *paciente = criar_pac(1);
	char *c = malloc(2 * sizeof(char));
	memset(c, '\0', 2 * sizeof(char));
	get_pac(paciente, 1, c);
	free(c);

	// Inicializa o horário para 08:00 (em minutos) e uma fila (deque) contendo outras 9 filas (deques), uma para cada especialidade médica.
	int horario = 8 * 60;
	dq **fila_medicos = malloc(9 * sizeof(dq *));
	for (int esp = 0; esp < 9; esp++)
		fila_medicos[esp] = criar_deque();

	// Distribui os pacientes para cada uma das 9 filas de especialidades, sem realizar nenhum atendimento.
	primeira_chamada(fila_medicos, paciente);

	// Realiza atendimentos e remaneja pacientes para a próxima especialidade.
	// Ao fim dos atendimentos de cada paciente, o dispensa e imprime seu horário de saída.
	// A função ciclo_de_atendimento() retorna 1 se e somente se algum paciente foi atendido naquele ciclo.
	// Durante o ciclo, a lista fila_medicos é esvaziada e seus pacientes desalocados da memória.
	// A função retornará 0 quando os pacientes houverem sido todas liberados.
	while(ciclo_de_atendimento(fila_medicos, &horario));

	for (int esp = 0; esp < 9; esp++) 
		free_deque(fila_medicos[esp]);
	free(fila_medicos);

	return 0;
}


/* Captura iterativamente o input de informações dos pacientes, caracter por caracter a partir de stdin.
*  A função foi dificultada pelo fato de não se saber de início quantos pacientes serão inseridos.
*  Cada paciente possui uma ID única, começando em 1 e sendo incrementada um-a-um. */
void get_pac(pac *paciente, int id, char *c) {
	DLL *atendimentos = criar_DLL();
	DLL *ptr_atend = atendimentos;

	while (scanf("%c", c) != EOF) {
		// Se o caracter não é uma quebra de linha (newspace), então contém informação um paciente.
		if (*c != '\n') {
			// Se o caracter é uma aspa ("), mais caracteres são coletados até encontrar outra aspa. Trata-se do nome do paciente.
			if (*c == '\"') {
				char *nome = malloc(101 * sizeof(char)); // Os nomes possuem até 100 caracteres.
				*nome = '\0';
				while ((*c = getchar()) != '\"')
					strcat(nome, c);
				(*paciente).nome = nome;
				continue;

			// Se o caracter é uma letra, então trata-se do tipo de prioridade do paciente.
			} else if ('a' <= *c && *c <= 'z') {
				char *prioridade = malloc(13 * sizeof(char)); // Os tipos de prioridade possuem até 10 caracteres ('normal', 'prioridade').
				*prioridade = '\0';
				strcat(prioridade, c);
				while ((*c = getchar()) != ' ')
					strcat(prioridade, c);
				(*paciente).prioridade = prioridade;
				continue;

			// Se o caracter é um dígito, trata-se de um atendimento do paciente.
			} else if ('0' <= *c && *c <= '9') {
				(*ptr_atend).val = *c - '0';
				(*ptr_atend).next = criar_DLL();
				(*ptr_atend).next->prev = ptr_atend;
				ptr_atend = (*ptr_atend).next;
				continue;
			}

		// Se o caracter é uma quebra de linha, então terminamos de inserir um paciente.
		} else {
			// Na captura do útimo atendimento, um espaço extra foi criado via criar_DLL() para um próximo (mas inexistente) atendimento.
			(*ptr_atend).prev->next = NULL;
			free(ptr_atend);
			// Insere os atendimenos do paciente e cria o próximo.
			id++;
			(*paciente).atendimentos = atendimentos;
			(*paciente).next = criar_pac(id);
			(*paciente).next->prev = paciente;
			return get_pac((*paciente).next, id, c);
		}
	}

	// Este segmento do código é executado somente quando o arquivo de input foi lido por completo.
	// Na última iteração, não há paciente a ser inserido, mas uma DLL de atendimentos foi criada mesmo assim.
	free(atendimentos);
	// Na última iteração, espaço foi criado para um próximo (mas inexistente) paciente.
	(*paciente).prev->next = NULL;
	free(paciente);

	return;
}

/* Percorre a lista ligada de pacientes e os insere nas filas de espera de cada uma das 9 especialidades médicas.
*  A inseração é feita de acordo com seu status prioridade. Nenhum atendimento é realizado. */
void primeira_chamada(dq **fila_medicos, pac *paciente) {
	while (paciente != NULL) {
		pac *proximo = (*paciente).next;
		int esp = (*paciente).atendimentos->val - 1;
		if (strcmp((*paciente).prioridade, "preferencial") == 0)
			insere_inicio(fila_medicos[esp], paciente);
		else
			insere_fim(fila_medicos[esp], paciente);
		paciente = proximo;
	}
	return;
}

/* Em turnos (ciclos) de dez minutos, realiza atendimentos e remaneja pacientes para a próxima especialidade.
*  A disponibilidade finita de médicos para atendimento é levada em conta, sendo resetada ao início de cada ciclo.
*  Além disso, ao fim de cada ciclo o horário é atualizado (em minutos), sendo acrescido de dez minutos.
*  Ao fim dos atendimentos de cada paciente, ele é dispensado e seu horário de saída é impresso.
*  A iteração procede até serem dispensados todos os pacientes. */
int ciclo_de_atendimento(dq **fila_medicos, int *horario) {
	// Se todas as filas de espera são percorridas e não houve atendimento, ciclo_de_atendimento() retornará 0.
	// Se houve atendimento, retornará 1 e o loop while() em main continuará.
	int houve_atendimento = 0;
	
	// Armazena a disponibilidade de médicos a cada ciclo.
	int medicos[9] = {10, 2, 5, 3, 4, 7, 2, 1, 4};

	// Cria uma fila temporária, alocando os pacientes para suas novas especialidades.
	// A fila original não pode ser alterada durante um ciclo de atendimento, visto que é preciso simular que todos os atendimentos terminem juntos.
	dq **nova_fila = malloc(9 * sizeof(dq *));
	for (int esp = 0; esp < 9; esp++)
		nova_fila[esp] = criar_deque();

	// Percorre cada uma das nove filas de espera, uma para cada especialidade.
	for (int esp = 0; esp < 9; esp++) {
		// Enquanto houver médicos disponíveis para aquela especialidade...
		while (medicos[esp] > 0) {
			// Atende o primeiro paciente da fila de espera, se houver.
			pac *paciente = (*fila_medicos[esp]).ini;
			if (paciente != NULL) {
				houve_atendimento = 1; // Se houve ao menos atendimento em ao menos uma fila, ciclo_de_atendimento() retornará 1 e o loop while() em main() continuará.
				medicos[esp]--; // Ocupa uma médico daquela especialidade.
				// Libera a memória dinamicamente alocada no armazenamento da especialidade que o paciente visitou.
				DLL *backup = (*paciente).atendimentos;
				(*paciente).atendimentos = backup->next;
				free(backup);
				// Remove o paciente da lista de espera atual.
				if ((*paciente).atendimentos != NULL) {
					// Se o paciente ainda possui atendimentos, ele é alocado para uma lista de espera temporária.
					int prox_esp = (*paciente).atendimentos->val - 1;
					mover_paciente(fila_medicos[esp], nova_fila[prox_esp]);
				} else {
					// Se o paciente não possui mais atendimenos, ele é liberado e seu horário de saída é impresso.
					print_atendimento(paciente, horario);
					remove_inicio(fila_medicos[esp]);
				}
			} else
				// Se não há pacientes a serem atendidos por aquela especialidade, passa-se para a próxima.
				break;
		}
	}

	// As filas remanescentes de cada especialidade precisam ser integradas com as filas temporárias que foram criadas.
	// Pacientes prioritários passarão na frente de todos, enquanto pacientes normais entram por último.
	// Todos os pacientes são remanejados de acordo com sua ordem de entrada inicial em get_pac(), armazenado em seu ID.
	for (int esp = 0; esp < 9; esp++) {
		fila_medicos[esp] = integrar_filas(fila_medicos[esp], nova_fila[esp], esp);
		free_deque(nova_fila[esp]);
	}
	free(nova_fila);

	// Ao fim de cada ciclo, o horário é acrescido de dez minutos.
	*horario += 10;

	return houve_atendimento;
}

/* Move um paciente de uma fila (deque) para outra, de acordo com seu status de preferência. */
void mover_paciente(dq *fila_antiga, dq *fila_nova) {
	pac *paciente = (*fila_antiga).ini;

	if ((*fila_antiga).ini->next == NULL) {
		// A fila foi totalmente esvaziada.
		(*fila_antiga).ini = NULL;
		(*fila_antiga).fim = NULL;
	} else {
		// A fila ainda contém pacientes.
		(*fila_antiga).ini = (*fila_antiga).ini->next;
		(*fila_antiga).ini->prev = NULL;
	}

	// Insere o paciente na fila no início ou no fi, a depender de seu status de prioridade.
	if (strcmp((*paciente).prioridade, "preferencial") == 0) 
		insere_inicio(fila_nova, paciente);
	else
		insere_fim(fila_nova, paciente);
	return;
}

/* Imprime em formado hh:mm o horário de saída de cada paciente, dado que a variável "horário" está em minutos. */
void print_atendimento(pac *paciente, int *horario) {
	int horas = (*horario + 10) / 60;
	int minutos = (*horario + 10) % 60;
	printf("%.2d:%.2d ", horas, minutos);
	printf("%s\n", (*paciente).nome);
	return;
}

/* As filas remanescentes de cada especialidade precisam ser integradas com as filas temporárias que foram criadas.
*  Pacientes prioritários passarão na frente de todos, enquanto pacientes normais entram por último.
*  Todos os pacientes são remanejados de acordo com sua ordem de entrada inicial em get_pac(), armazenado em seu ID. */
dq *integrar_filas(dq *velho, dq* novo, int esp) {
	// Se nenhum paciente foi remanejado, preenchendo assim a fila temporária (novo), a fila antiga remanescente (velho) não é alterada. 
	if ((*novo).ini == NULL)
		return velho;

	// A fila temporária (novo) não é vazia, e portanto deverá haver remanejamento.
	else {
		// A lista "integrado" será devolvida no final, após ser preenchida na ordem apropriada com os pacientes das duas listras de entrada.
		dq *integrado = criar_deque();

		// Contagem de quantos pacientes prioritários e normais há na fila temporária (novo). Os pacientes preferenciais estão à frente na fila.
		pac *ptr_novo = (*novo).ini;
		int num_prioridade = 0;
		while (strcmp((*ptr_novo).prioridade, "preferencial") == 0) {
			num_prioridade++;
			ptr_novo = (*ptr_novo).next;
			if (ptr_novo == NULL)
				break;
		}
		int num_normal = 0;
		while (ptr_novo != NULL) {
			num_normal++;
			ptr_novo = (*ptr_novo).next;
		}

		// Os vetores id_prioridade e id_normal armazenarão os IDs dos pacientes prioritários e normais da fila temporária (novo).
		int *id_prioridade = malloc(num_prioridade * sizeof(int));
		int *id_normal = malloc(num_normal * sizeof(int));
		ptr_novo = (*novo).ini;
		for (int i = 0; i < num_prioridade; i++) {
			id_prioridade[i] = (*ptr_novo).id;
			ptr_novo = (*ptr_novo).next;
		}
		for (int i = 0; i < num_normal; i++) {
			id_normal[i] = (*ptr_novo).id;
			ptr_novo = (*ptr_novo).next;
		}
		
		// Os vetores armazenando os IDs dos pacientes serão ordenados de forma crescente.
		qsort(id_prioridade, num_prioridade, sizeof(int), cmpfunc);
		qsort(id_normal, num_normal, sizeof(int), cmpfunc);
		
		// Os pacientes prioritários da nova fila são copiados e inseridos na lista integrada, dando preferência àqueles com ID menor.
		for (int i = 0; i < num_prioridade; i++) {
			// O paciente com o ID aprioriado, armazenado em id_prioridade ordenada, será copiado da lista original e inserido na lista integrada.
			pac *paciente = copiar_pac_isolado_id((*novo).ini, id_prioridade[i]);
			insere_fim(integrado, paciente);
		}

		// A velha fila é simplesmente concatenada à lista integrada.
		if ((*velho).ini != NULL) {
			if ((*integrado).ini == NULL) {
				(*integrado).ini = (*velho).ini;
				(*integrado).fim = (*velho).fim;
			} else {
				(*integrado).fim->next = (*velho).ini;
				(*velho).ini-> prev = (*integrado).fim;
				(*integrado).fim = (*velho).fim;
			}
		}
		free(velho);

		// Os pacientes normais da nova fila são copiados e inseridos na lista integrada, dando preferência àqueles com ID menor.
		for (int i = 0; i < num_normal; i++) {
			// O paciente com o ID aprioriado, armazenado em id_normal ordenada, será copiado da lista original e inserido na lista integrada.
			pac *paciente = copiar_pac_isolado_id((*novo).ini, id_normal[i]);
			insere_fim(integrado, paciente);
		}

		free(id_prioridade);
		free(id_normal);

		return integrado;
	}
}

// Função padrão de comparação de inteiros para qsort(), retirada da internet conforme permitido no enunciado.
int cmpfunc(const void * a, const void * b) {
   return *(int *)a - *(int *)b;
}