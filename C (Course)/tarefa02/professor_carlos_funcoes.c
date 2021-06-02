/***************************************************************
* MC202 E - 2S/2020 
* Aluno: Daniel Credico de Coimbra
* RA: 155077
* Data: 23 de Setembro de 2020
* Descrição: Tarefa 02 | Professor Carlos
* Implementa as operações que caracterizam os Tipos Abstratos de
* Dados (TADs) 'Data', 'Aluno', e 'Turma', fazendo uso de funções
* auxiliares escondidas (prefaciadas por _).
****************************************************************/


#include <stdio.h>
#include "professor_carlos.h"

int _agecmp(Data A, Data B);
int _lexcmp(Aluno A, Aluno B);
int _strcmp(char *nomeA, char *nomeB);
int _substr(char *padrao, char *nome);

Aluno procura_novo_na_turma(Turma t[], int qtd_turmas, int j) {
	/* Recebe todas as turmas e procura o aluno mais novo na turma j.
	*  Em caso de empate de idade, procura-se o aluno com menor nome
	*  e sobrenome na ordem lexicográfica.
	*/
	int id_aluno_novo = 0;
	Data menor_idade = t[j].alunos[id_aluno_novo].nascimento;
	int comparacao_data;
	int comparacao_lexico;

	for (int i = 1; i < t[j].qtd; i++) {
		comparacao_data = _agecmp(t[j].alunos[i].nascimento, menor_idade);
		switch (comparacao_data) {
			case 1:
				id_aluno_novo = i;
				menor_idade = t[j].alunos[i].nascimento;
				break;

			case -1:
				break;

			case 0:
				comparacao_lexico = _lexcmp(t[j].alunos[i], t[j].alunos[id_aluno_novo]);
				if (comparacao_lexico == -1) {
					id_aluno_novo = i;
					menor_idade = t[j].alunos[i].nascimento;
				} else {
					continue;
				}
		}
	}

	return t[j].alunos[id_aluno_novo];
}

Aluno procura_novo_todas_turmas(Turma t[], int qtd_turmas) {
	/* Recebe todas as turmas e determina o aluno mais novo de cada
	*  turma; depois determina o aluno mais novo dentre todos esses.
	*  Os critérios para ser o aluno mais novo são dados em
	*  procura_novo_na_turma(), que é empregada nesta função.
	*/
	Turma mais_novos;
	mais_novos.qtd = qtd_turmas;
	
	for (int j = 0; j < qtd_turmas; j++)
		mais_novos.alunos[j] = procura_novo_na_turma(t, qtd_turmas, j);

	Turma mais_novos_vetor[1] = {mais_novos};

	return procura_novo_na_turma(mais_novos_vetor, 1, 0);
}

Aluno procura_velho_na_turma(Turma t[], int qtd_turmas, int j) {
	/* Recebe todas as turmas e procura o aluno mais velho na turma j.
	*  Em caso de empate de idade, procura-se o aluno com menor nome
	*  e sobrenome na ordem lexicográfica.
	*/
	int id_aluno_velho = 0;
	Data maior_idade = t[j].alunos[id_aluno_velho].nascimento;
	int comparacao_data;
	int comparacao_lexico;
	
	for (int i = 1; i < t[j].qtd; i++) {
		comparacao_data = _agecmp(t[j].alunos[i].nascimento, maior_idade);
		switch (comparacao_data) {
			case -1:
				id_aluno_velho = i;
				maior_idade = t[j].alunos[i].nascimento;
				break;

			case 1:
				break;

			case 0:
				comparacao_lexico = _lexcmp(t[j].alunos[i], t[j].alunos[id_aluno_velho]);
				if (comparacao_lexico == -1) {
					id_aluno_velho = i;
					maior_idade = t[j].alunos[i].nascimento;
				} else {
					continue;
				}
		}
	}

	return t[j].alunos[id_aluno_velho];
}

Aluno procura_velho_todas_turmas(Turma t[], int qtd_turmas) {
	/* Recebe todas as turmas e determina o aluno mais velho de cada
	*  turma; depois determina o aluno mais velho dentre todos esses.
	*  Os critérios para ser o aluno mais velho são dados em
	*  procura_velho_na_turma(), que é empregada nesta função.
	*/
	Turma mais_velhos;
	mais_velhos.qtd = qtd_turmas;
	for (int j = 0; j < qtd_turmas; j++)
		mais_velhos.alunos[j] = procura_velho_na_turma(t, qtd_turmas, j);

	Turma mais_velhos_vetor[1] = {mais_velhos};

	return procura_velho_na_turma(mais_velhos_vetor, 1, 0);
}

int conta_substrings(Turma t[], int qtd_turmas, char *padrao) {
	/* Enumera todos os alunos dentre todas as turmas cujos nomes
	*  (não sobrenomes) contêm a string refenciada pelo parâmetro
	*  'padrão'.  Emprega _substr() para determinar se uma string
	*  está contida em outra.
	*/
	int total = 0;

	for (int i = 0; i < qtd_turmas; i++) {
		for (int j = 0; j < t[i].qtd; j++) {
			total += _substr(padrao, t[i].alunos[j].nome);
		}
	}

	return total;
}

int add_aluno(Turma t[], Aluno A, int j) {
	/* Adiciona um aluno a uma turma e devolve o número resultante de alunos. */
	t[j].alunos[t[j].qtd++] = A;
	
	return t[j].qtd;
}

int remove_aluno(Turma t[], int j) {
	/* Diminui em um  a quantia registrada de alunos na turma j, o que é
	*  equivalente na prática a remover o último aluno da turma j.
	*/
	return --t[j].qtd;
}

int _agecmp(Data A, Data B) {
	/* Retorna -1 se a primeira Data é anterior.
	*  Retorna +1 se a segunda  Data é anterior.
	*  Retorna  0 se as Datas são a mesma.
	*/
	if (A.ano > B.ano) {
		return 1;
	} else if (A.ano < B.ano) {
		return -1;
	} else {
		if (A.mes > B.mes) {
			return 1;
		} else if (A.mes < B.mes) {
			return -1;
		} else {
			if (A.dia > B.dia) {
				return 1;
			} else if (A.dia < B.dia) {
				return -1;
			} else {
				return 0;
			}
		}
	}
}

int _lexcmp(Aluno A, Aluno B) {
	/* Retorna -1 se o nome/sobrenome do primeiro Aluno for anterior (ordem lexicográfica).
	*  Retorna +1 se o nome/sobrenome do primeiro Aluno for anterior (ordem lexicográfica).
	*  Retorna 0 como default, mas esse caso não surge nas condições da tarefa.
	*/

	int comparacao_nome;
	int comparacao_sobrenome;

	comparacao_nome = _strcmp(A.nome, B.nome);
	switch (comparacao_nome) {
		case 1:
			return 1;

		case -1:
			return -1;

		case 0:
			comparacao_sobrenome = _strcmp(A.sobrenome, B.sobrenome);
			if (comparacao_sobrenome == 1)
				return 1;
			else
				return -1;

		default:
			return 0;
	}
}

int _strcmp(char *strA, char *strB) {
	/* Retorna -1 se a primeira string é menor (ordem lexicográfica).
	*  Retorna +1 se a segunda  string é menor (ordem lexicográfica)
	*  Retorna  0 se as strings são iguais.
	*  Uma string inteiramente contida na outra é considerada menor (e.g. "Lia" em "Liane").
	*/
	for (int i = 0; i < 15; i++) {
		if (strA[i] == '\0' && strB[i] != '\0') {
			return -1;
		} else if (strA[i] != '\0' && strB[i] == '\0') {
			return 1;
		} else if (strA[i] == '\0' && strB[i] == '\0') {
			return 0;
		} else {
			if (strA[i] < strB[i])
				return -1;
			else if (strA[i] > strB[i])
				return 1;
			else
				continue;
		}
	}

	return 0;
}

int _substr(char *padrao, char *nome) {
	/* Determina se uma string está contida dentro de outra.
	*  Retorna 1 se estiver contida, retorna 0 se não estiver.
	*/
	int nome_len = -1;
	int padrao_len = -1;
	while (nome[++nome_len]);  // '\0' é avaliado como zero (false), terminando a contagem.
	while (padrao[++padrao_len]);  // '\0' é avaliado como zero (false), terminando a contagem.

	if (nome_len < padrao_len)
		return 0;

	for (int i = 0; i < nome_len; i++) {
		if (nome[i] != padrao[0]) {
			continue;
		} else {
			for (int j = 1; j <= padrao_len; j++) {
				if (j == padrao_len) {
					return 1;
				} else if (nome[i + j] != padrao[j]) {
					break;
				} else {
					continue;
				}
			}
		}
	}

	return 0;
}