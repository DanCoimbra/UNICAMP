/*********************************************************
* MC202 E - 2S/2020 
* Aluno: Daniel Credico de Coimbra
* RA: 155077
* Data: 19 de Setembro de 2020
* Descrição: Tarefa 01 | Mega da Virada
* Simulador de apostas na Mega Sena da Virada com até mil
* apostadores e com prêmios para quadra, quina, e sena.
**********************************************************/

#include <stdio.h>

void preencher_aposta(int apostador, int apostas[1000][15], int volante[6][10]);
int get_acertos(int aposta[15], int sorteio[6]);


int main(void) {
	int num_apostadores;
	double valor_premio;
	scanf("%d %lf", &num_apostadores, &valor_premio);

	int apostas[1000][15] = {0}; // Até 15 números escolhidos por cada apostador
	int perfil_apostadores[1000][3] = {0}; // Quadras, quinas, ou senas por apostador

	/* Preenche as cartelas (volantes) de cada apostador */
	for (int apostador = 0; apostador < num_apostadores; apostador++) {
		int volante[6][10];
		for (int row = 0; row < 6; row++) {
			for (int col = 0; col < 10; col++) {
				int escolha;
				scanf("%d", &escolha);
				volante[row][col] = escolha;
			}
		}
		preencher_aposta(apostador, apostas, volante);
	}

	/* Determina os seis números sorteados */
	int sorteio[6];
	for (int i = 0; i < 6; i++) {
		int num_sorteado;
		scanf("%d", &num_sorteado);
		sorteio[i] = num_sorteado;
	}

	/* Determina quantos apostadores acertaram quadra, quina, ou sena */
	double quadras = 0.0;
	double quinas = 0.0;
	double senas = 0.0;
	for (int apostador = 0; apostador < num_apostadores; apostador++) {
		int acertos;
		acertos = get_acertos(apostas[apostador], sorteio);
		if (acertos == 4) {
			quadras++;
			perfil_apostadores[apostador][0]++;
		} else if (acertos == 5) {
			quinas++;
			perfil_apostadores[apostador][1]++;
		} else if (acertos == 6) {
			senas++;
			perfil_apostadores[apostador][2]++;
		}
	}

	/* Determina o valor do prêmio para quadra, quina, e sena */
	double premio_quadras, premio_quinas, premio_senas;
	if (quadras > 0) {
		premio_quadras = (valor_premio * 0.19) / quadras;
	}
	if (quinas > 0) {
		premio_quinas = (valor_premio * 0.19) / quinas;
	}
	if (senas > 0) {
		premio_senas = (valor_premio * 0.62) / senas;
	}

	/* Demonstra na tela o valor do prêmio recebido por cada apostador */
	for (int apostador = 0; apostador < num_apostadores; apostador++) {
		if (perfil_apostadores[apostador][0] == 1)
			printf("%.2lf\n", premio_quadras);
		else if (perfil_apostadores[apostador][1] == 1)
			printf("%.2lf\n", premio_quinas);
		else if (perfil_apostadores[apostador][2] == 1)
			printf("%.2lf\n", premio_senas);
		else
			printf("%.2lf\n", 0.0);	
	}

	return 0;
}


void preencher_aposta(int apostador, int apostas[1000][15], int volante[6][10]){
	/* Preenche vetor de até 15 números selecionados por um apostador,
	*  transformando marcações no volante (matriz 6x10) em números.
	*/

	int total_apostas = 0;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 10; j++) {
			if (volante[i][j] == 1) {
				int numero_apostado = i*10 + (j + 1);
				apostas[apostador][total_apostas] = numero_apostado;
				total_apostas++;
			}
		}
	}

	return;
}


int get_acertos(int aposta[15], int sorteio[6]) {
	/* Determina quantos números selecionados por um apostador estão dentre
	*  os números selecionados. Devolve o valor inteiro do número de acertos.
	*  Como zero não é um número selecionável, sua aparição no vetor aposta[]
	*  marca o final dos números selecionados.
	*/

	int acertos = 0;
	int i = 0;
	while (i < 15 && aposta[i] != 0) {
		for (int j = 0; j < 6; j++) {
			if (aposta[i] == sorteio[j])
				acertos++; 
		}
		i++;
	}

	return acertos;	
}