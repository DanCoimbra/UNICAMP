/***************************************************************
* MC202 E - 2S/2020 
* Aluno: Daniel Credico de Coimbra
* RA: 155077
* Data: 23 de Setembro de 2020
* Descrição: Tarefa 02 | Professor Carlos
* Cliente que Professor Carlos usará para empregar os Tipos
* Abstratos de Dados (TADs) 'Data', Aluno', e 'Turma' para obter
* informações automaticamente sobre suas turmas.
****************************************************************/

#include <stdio.h>
#include "professor_carlos.h"

int main(void) {
	/* Obtém os valores n e k do enunciado. */
	int qtd_turmas;
	int qtd_ops;
	scanf("%d %d", &qtd_turmas, &qtd_ops);

	/* Cria e preenche as turmas com alunos e seus dados. */
	Turma turmas[50];
	Aluno pessoa;
	int qtd_alunos;
	for (int indice_turma = 0; indice_turma < qtd_turmas; indice_turma++) {
		Turma sala;
		scanf("%d", &qtd_alunos);
		sala.qtd = qtd_alunos;
		for (int aluno = 0; aluno < qtd_alunos; aluno++) {
			scanf("%s", pessoa.nome);
			scanf("%s", pessoa.sobrenome);
			scanf("%d", &pessoa.nascimento.dia);
			scanf("%d", &pessoa.nascimento.mes);
			scanf("%d", &pessoa.nascimento.ano);
			sala.alunos[aluno] = pessoa;
		}
		turmas[indice_turma] = sala;
	}

	/* Determina e chama a operação escolhida pelo Professor Carlos, então imprime o resultado */
	int codigo_op;
	int indice_turma;
	int inteiro_retorno;
	Aluno aluno_retorno;
	Aluno aluno_entrada;
	char padrao[6];
	for (int op = 0; op < qtd_ops; op++) {
		scanf("%d", &codigo_op);
		switch (codigo_op) {
			case 1:
				scanf("%d", &indice_turma);
				aluno_retorno = procura_novo_na_turma(turmas, qtd_turmas, indice_turma);
				printf("%s\n", aluno_retorno.nome);
				break;

			case 2:
				scanf("%d", &indice_turma);
				aluno_retorno = procura_velho_na_turma(turmas, qtd_turmas, indice_turma);
				printf("%s\n", aluno_retorno.sobrenome);
				break;

			case 3:
				aluno_retorno = procura_velho_todas_turmas(turmas, qtd_turmas);
				printf("%s\n", aluno_retorno.nome);
				break;

			case 4:
				aluno_retorno = procura_novo_todas_turmas(turmas, qtd_turmas);
				printf("%s\n", aluno_retorno.sobrenome);
				break;

			case 5:
				scanf("%s", padrao);
				inteiro_retorno = conta_substrings(turmas, qtd_turmas, padrao);
				printf("%d\n", inteiro_retorno);
				break;

			case 6:
				scanf("%d", &indice_turma);
				scanf("%s", aluno_entrada.nome);
				scanf("%s", aluno_entrada.sobrenome);
				scanf("%d", &aluno_entrada.nascimento.dia);
				scanf("%d", &aluno_entrada.nascimento.mes);
				scanf("%d", &aluno_entrada.nascimento.ano);
				inteiro_retorno = add_aluno(turmas, aluno_entrada, indice_turma);
				turmas[indice_turma].qtd = inteiro_retorno;
				printf("%d\n", inteiro_retorno);
				break;

			case 7:
				scanf("%d", &indice_turma);
				inteiro_retorno = remove_aluno(turmas, indice_turma);
				turmas[indice_turma].qtd = inteiro_retorno;
				printf("%d\n", inteiro_retorno);
				break;
		}
	}

	return 0;
}