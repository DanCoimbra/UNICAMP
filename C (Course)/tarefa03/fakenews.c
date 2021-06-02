/***************************************************************
* MC202 E - 2S/2020 
* Aluno: Daniel Credico de Coimbra
* RA: 155077
* Data: 28 de Setembro de 2020
* Descrição: Tarefa 03 | Fake News
* Classifica termos de pesquisa digital com base em suas
* estatísticas de relevância de pesquisa, determinando se
* tais estatísticas se devem à atuaçao de bots ou, se não,
* se ao menos são de alguma forma fora do normal.
****************************************************************/


#include <stdio.h>
#include <stdlib.h>

/* Termos são registrados em estruturas contendo seu nome, série histórica de pesquisa, e propriedades estatísticas dessa série. */
typedef struct _Termo {
	char *nome;
	double *dados;
	double max;
	double min;
	double avg;
	double std;
} Termo;

Termo * get_termo(int qtd_dias);
double get_max(double *dados, int qtd_dias);
double get_min(double *dados, int qtd_dias);
double get_avg(double *dados, int qtd_dias);
double get_std(double *dados, int qtd_dias);
double get_sqrt(double num);
double get_abs(double num);
void print_bots(Termo **vetor_termos, int qtd_termos);
void print_surpreendentes(Termo **vetor_termos, int qtd_termos);
void print_normais(Termo **vetor_termos, int qtd_termos);
void print_locais(Termo **vetor_termos, int qtd_termos);
void print_irrelevantes(Termo **vetor_termos, int qtd_termos);


int main(void) {
	/* Obtém os os valores N e M do enunciado. */
	int qtd_termos;
	int qtd_dias;
	scanf("%d %d", &qtd_termos, &qtd_dias);
	
	/* Inicializa e aloga dinamicamente uma lista dos termos de pesquisa. */
	Termo **vetor_termos = malloc(qtd_termos * sizeof(Termo *));
	
	/* Obtém e calcula propriedades estatísticas dos termos e os imprime. */
	for (int i = 0; i < qtd_termos; i++) {
		vetor_termos[i] = get_termo(qtd_dias);
		printf("%s ", (*vetor_termos[i]).nome);
		printf("%.2lf ",(*vetor_termos[i]).max);
		printf("%.2lf ", (*vetor_termos[i]).min);
		printf("%.2lf ", (*vetor_termos[i]).avg);
		printf("%.2lf\n", (*vetor_termos[i]).std);
	}

	/* Imprime as classificações acerca do histórico de relevância dos termos. */
	printf("\nRESULTADO:\n");
	print_bots(vetor_termos, qtd_termos);
	print_surpreendentes(vetor_termos, qtd_termos);
	print_normais(vetor_termos, qtd_termos);
	print_locais(vetor_termos, qtd_termos);
	print_irrelevantes(vetor_termos, qtd_termos);

	/* Libera espaços de memória alocados dinamicamente. */
	for (int i = 0; i < qtd_termos; i++) {
		free((*vetor_termos[i]).nome);
		free((*vetor_termos[i]).dados);
		free(vetor_termos[i]);
	}

	free(vetor_termos);

	return 0;
}


Termo * get_termo(int qtd_dias) {
	/* Obtém os termos, seus dados de pesquisa, e suas propriedades estatísticas. */
	char *nome = malloc(25 * sizeof(char));
	scanf("%s ", nome);

	double *dados = malloc(qtd_dias * sizeof(double));
	for (int i = 0; i < qtd_dias; i++)
		scanf("%lf", &dados[i]);

	Termo *termo = malloc(sizeof(Termo));
	(*termo).nome = nome;
	(*termo).dados = dados;
	(*termo).max = get_max(dados, qtd_dias);
	(*termo).min = get_min(dados, qtd_dias);
	(*termo).avg = get_avg(dados, qtd_dias);
	(*termo).std = get_std(dados, qtd_dias);

	return termo;
}

double get_max(double *dados, int qtd_dias) {
	/* Determina a maior relevância de pesquisa na série histórica. */
	double max = dados[0];

	for (int i = 1; i < qtd_dias; i++)
		if (dados[i] > max)
			max = dados[i];

	return max;
}

double get_min(double *dados, int qtd_dias) {
	/* Determina a menor relevância de pesquisa na série histórica. */
	double min = dados[0];

	for (int i = 1; i < qtd_dias; i++)
		if (dados[i] < min)
			min = dados[i];

	return min;
}

double get_avg(double *dados, int qtd_dias) {
	/* Determina a média de relevância de pesquisa na série histórica. */
	double avg = 0;

	for (int i = 0; i < qtd_dias; i++)
		avg += dados[i];

	return avg /= qtd_dias;
}

double get_std(double *dados, int qtd_dias) {
	/* Determina o desvio-padrão de relevância de pesquisa na série histórica. */
	double std = 0;
	double avg = get_avg(dados, qtd_dias);

	for (int i = 0; i < qtd_dias; i++)
		std += (dados[i] - avg) * (dados[i] - avg);

	std /= qtd_dias;
	std = get_sqrt(std);

	return std;
}

double get_sqrt(double num) {
	/* Determina com precisão suficiente a raíz quadrada de um número. */
	double error = 0.0001;
	double guess = num;
	double prev_guess;

	do {
		prev_guess = guess;
		guess += (num / guess);
		guess /= 2;
	} while (error < get_abs(guess - prev_guess));

	return guess;
}

double get_abs(double num) {
	/* Determina o módulo de um número. */
	if (num > 0)
		return num;
	else
		return -num;
}

void print_bots(Termo **vetor_termos, int qtd_termos) {
	/* Determina se o histórico de relevância do termo é indicativo de atuação de bots, e imprime o resultado. */
	int total = 0;
	int *bots = malloc(qtd_termos * sizeof(int));
	for (int i = 0; i < qtd_termos; i++)
		bots[i] = 0;

	for (int i = 0; i < qtd_termos; i++)
		if ((*vetor_termos[i]).avg >= 60 && (*vetor_termos[i]).std > 15) {
			bots[i] = 1;
			total++;
		}

	printf("Bot (%d):", total);
	for (int i = 0; i < qtd_termos; i++)
		if (bots[i] == 1)
			printf(" %s", (*vetor_termos[i]).nome);
	printf("\n");

	free(bots);

	return;
}

void print_surpreendentes(Termo **vetor_termos, int qtd_termos) {
	/* Determina se o histórico de relevância do termo é ao menos surpreendente, e imprime o resultado na tela. */
	int total = 0;
	int *surpreendentes = malloc(qtd_termos * sizeof(int));
	for (int i = 0; i < qtd_termos; i++)
		surpreendentes[i] = 0;

	for (int i = 0; i < qtd_termos; i++)
		if ((*vetor_termos[i]).avg >= 60 && (*vetor_termos[i]).std <= 15) {
			surpreendentes[i] = 1;
			total++;
		}

	printf("Surpreendente (%d):", total);
	for (int i = 0; i < qtd_termos; i++)
		if (surpreendentes[i] == 1)
			printf(" %s", (*vetor_termos[i]).nome);
	printf("\n");

	free(surpreendentes);

	return;
}

void print_normais(Termo **vetor_termos, int qtd_termos) {
	/* Determina se o histórico de relevância do termo é normal, e imprime o resultado na tela. */
	int total = 0;
	int *normais = malloc(qtd_termos * sizeof(int));
	for (int i = 0; i < qtd_termos; i++)
		normais[i] = 0;

	for (int i = 0; i < qtd_termos; i++)
		if ((*vetor_termos[i]).avg < 60 && (*vetor_termos[i]).max >= 80 && (*vetor_termos[i]).min > 20) {
			normais[i] = 1;
			total++;
		}

	printf("Normal (%d):", total);
	for (int i = 0; i < qtd_termos; i++)
		if (normais[i] == 1)
			printf(" %s", (*vetor_termos[i]).nome);
	printf("\n");

	free(normais);

	return;
}

void print_locais(Termo **vetor_termos, int qtd_termos) {
	/* Determina se um termo possui relevância meramente local, e imprime o resultado na tela. */
	int total = 0;
	int *locais = malloc(qtd_termos * sizeof(int));
	for (int i = 0; i < qtd_termos; i++)
		locais[i] = 0;

	for (int i = 0; i < qtd_termos; i++)
		if ((*vetor_termos[i]).avg < 60 && (*vetor_termos[i]).max >= 80 && (*vetor_termos[i]).min <= 20) {
			locais[i] = 1;
			total++;
		}

	printf("Local (%d):", total);
	for (int i = 0; i < qtd_termos; i++)
		if (locais[i] == 1)
			printf(" %s", (*vetor_termos[i]).nome);
	printf("\n");

	free(locais);

	return;
}

void print_irrelevantes(Termo **vetor_termos, int qtd_termos) {
	/* Determina se um termo é irrelevante, e imprime o resultado na tela. */
	int total = 0;
	int *irrelevantes = malloc(qtd_termos * sizeof(int));
	for (int i = 0; i < qtd_termos; i++)
		irrelevantes[i] = 0;

	for (int i = 0; i < qtd_termos; i++)
		if ((*vetor_termos[i]).avg < 60 && (*vetor_termos[i]).max < 80) {
			irrelevantes[i] = 1;
			total++;
		}

	printf("Irrelevante (%d):", total);
	for (int i = 0; i < qtd_termos; i++)
		if (irrelevantes[i] == 1)
			printf(" %s", (*vetor_termos[i]).nome);
	printf("\n");

	free(irrelevantes);

	return;
}