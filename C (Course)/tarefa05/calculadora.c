/***************************************************************
* MC202 E - 2S/2020 
* Aluno: Daniel Credico de Coimbra
* RA: 155077
* Data: 29 de Outubro de 2020
* Descrição: Tarefa 05 | Calculadora de Números Gigantes
* Implementa as operações básicas de uma calculadora de números
* naturais arbitrariamente grandes, com armazenamento digito-a-
* digito em uma lista ligada. Implementa adição, subtração em
* módulo, multiplicação, e divisão inteira.
****************************************************************/

#include <stdio.h>
#include <stdlib.h>

// Supernúmero: cada dígito é armazenado como um nódulo numa lista ligada.
typedef struct snum {
	int digit;
	struct snum *next;
} snum;

// Caso: Lista ligada em que cada nódulo armazena uma instrução do input: dois supernúmeros e a operação a ser realizada.
typedef struct caso {
	char op;
	snum *val1;
	snum *val2;
	struct caso *next;
} caso;

// Supermatriz: uma lista ligada em que cada nódulo é um supernúmero.
typedef struct smatrix {
	snum *snumber;
	struct smatrix *next;
} smatrix;

snum *create_snum(void);
snum *get_snum(char breakpoint);
void free_snum(snum *value);
caso *create_caso(void);
void free_caso(caso *lista);
smatrix *create_smatrix(void);
void free_smatrix(smatrix *s_matrix);

snum *copy_snum(const snum *original);
void append_snum(snum *val1, snum *val2);
void print_snum(snum *value);
snum *remove_leading_zeros(snum *val);
snum *inverter_recursivo(const snum *original, int topo);

snum *superadd(snum *val1, snum *val2, int carry);
snum *supermult(snum *val1, snum *val2);
snum *aux_supermult(snum *val1, snum *val2, int carry);
snum *superdiv(snum *numerator, snum *denominator, snum *quotient);
snum *supersub(snum *val1, snum *val2, int carry);
int greater_than(snum *val1, snum *val2);
int more_digits_than(snum *val1, snum *val2);
int aux_greater_than(snum *val1, snum *val2);


int main(void) {
	// Recolhe o número de casos a serem inseridos.
	int num_casos;
	scanf("%d ", &num_casos);
	caso *lista_casos = create_caso();
	caso *caso_atual = lista_casos;
	caso *caso_proximo;

	// Recolhe a operação e os supernúmeros (via get_snum()) de cada caso. 
	char c;
	for (int caso = 1; caso <= num_casos; caso++) {
		scanf("%c ", &c);
		(*caso_atual).op = c;
		(*caso_atual).val1 = get_snum(' '); // O primeiro supernúmero é seguido de um espaço vazio.
		(*caso_atual).val2 = get_snum('\n'); // O segundo supernúmero é seguido de um caracter de nova linha.
		if (caso < num_casos) {
			caso_proximo = create_caso();
			(*caso_atual).next = caso_proximo;
			caso_atual = (*caso_atual).next;
		}
	}

	// Realiza as operações designadas em cada caso.
	caso_atual = lista_casos;
	while (caso_atual != NULL) {
		// Os casos +, –, e * invertem os supernúmeros para simplificar o algoritmo.
		char op = (*caso_atual).op;
		snum *res;

		if (op == '+') {
			snum *val1 = inverter_recursivo((*caso_atual).val1, 1);
			snum *val2 = inverter_recursivo((*caso_atual).val2, 1);
			snum *res_inv = superadd(val1, val2, 0);
			free_snum(val1);
			free_snum(val2);
			res = inverter_recursivo(res_inv, 1);
			free_snum(res_inv);
		}

		else if (op == '-') {
			// Como a subtração é em módulo, subtrai-se o menor do maior supernúmero para simplificar o algoritmo.
			snum *res_inv;
			if (greater_than((*caso_atual).val1, (*caso_atual).val2) > 0) {
				snum *val1 = inverter_recursivo((*caso_atual).val1, 1);
				snum *val2 = inverter_recursivo((*caso_atual).val2, 1);
				res_inv = supersub(val1, val2, 0);
				free_snum(val1);
				free_snum(val2);
			} else {
				snum *val1 = inverter_recursivo((*caso_atual).val1, 1);
				snum *val2 = inverter_recursivo((*caso_atual).val2, 1);
				res_inv = supersub(val2, val1, 0);
				free_snum(val1);
				free_snum(val2);
			}
			res = inverter_recursivo(res_inv, 1);
			free_snum(res_inv);
		}

		else if (op == '*') {
			snum *val1 = inverter_recursivo((*caso_atual).val1, 1);
			snum *val2 = inverter_recursivo((*caso_atual).val2, 1);
			snum *res_inv = supermult(val1, val2);
			free_snum(val1);
			free_snum(val2);
			res = inverter_recursivo(res_inv, 1);
			free_snum(res_inv);
		}

		else if (op == '/') {
			// O cálculo da divisão inteira é facilitado considerando os números não-invertidos.
			snum *val1 = copy_snum((*caso_atual).val1); // Será desalocado durante a função superdiv().
			snum *val2 = copy_snum((*caso_atual).val2);
			snum *quotient = create_snum();
			quotient = superdiv(val1, val2, quotient);
			res = quotient;
			free_snum(val2);
			//res = superdiv((*caso_atual).val1, (*caso_atual).val2, res);
		}

		// Remove zeros à esquerda e imprime o resultado, passando então ao cômputo do próximo caso.
		res = remove_leading_zeros(res);
		print_snum(res);
		free_snum(res);
		caso_atual = (*caso_atual).next;
	}
	free_caso(lista_casos);

	return 0;
}

// Aloca dinamicamente um supernúmero.
snum *create_snum(void) {
	snum *snumber = malloc(sizeof(snum));
	(*snumber).digit = 0;
	(*snumber).next = NULL;
	return snumber;
}

// Recolhe os dados de um supernúmero a partir do input textual.
snum *get_snum(char breakpoint) {
	snum *snumber = create_snum();
	snum *digito_atual = snumber;
	snum *digito_proximo;
	char c;
	scanf("%c", &c);
	for (;;) {
		(*digito_atual).digit = c - 48;
		scanf("%c", &c);
		if (c == breakpoint)
			break;
		else {
			digito_proximo = create_snum();
			(*digito_atual).next = digito_proximo;
			digito_atual = (*digito_atual).next;
		}
	}
	return snumber;
}

// Libera a memória dinamicamente alocada de um supernúmero. 
void free_snum(snum *value) {
	if ((*value).next != NULL)
		free_snum((*value).next);
	free(value);
	return;
}

// Aloca dinamicamente um caso.
caso *create_caso(void) {
	caso *lista = malloc(sizeof(caso));
	(*lista).val1 = NULL;
	(*lista).val2 = NULL;
	(*lista).next = NULL;
	return lista;
}

// Libera a memória dinamicamente alocada de um caso.
void free_caso(caso *lista) {
	if ((*lista).val1 != NULL)
		free_snum((*lista).val1);
	if ((*lista).val2 != NULL)
		free_snum((*lista).val2);
	if ((*lista).next != NULL)
		free_caso((*lista).next);
	free(lista);
	return;
}

// Aloca dinamicamente uma supermatriz.
smatrix *create_smatrix(void) {
	smatrix *s_matrix = malloc(sizeof(smatrix));
	(*s_matrix).snumber = create_snum();
	(*s_matrix).next = NULL;
	return s_matrix;
}

// Libera a memória dinamicamente alocada de uma supermatriz.
void free_smatrix(smatrix *s_matrix) {
	free_snum((*s_matrix).snumber);
	if ((*s_matrix).next != NULL)
		free_smatrix((*s_matrix).next);
	free(s_matrix);
	return;
}

// Copia um supernúmero em uma variável dinamicamente alocada em outro lugar.
snum *copy_snum(const snum *original) {
	snum *cpy = create_snum();
	snum *ptr_cpy = cpy;
	while (original != NULL) {
		(*ptr_cpy).digit = (*original).digit;
		if ((*original).next != NULL)
			(*ptr_cpy).next = create_snum();
		ptr_cpy = (*ptr_cpy).next;
		original = (*original).next;
	}
	return cpy;
}

// Concatena um supernúmero a outro.
void append_snum(snum *val1, snum *val2) {
	while ((*val1).next != NULL)
		val1 = (*val1).next;
	(*val1).next = val2;
	return;
}

// Imprime um supernúmero.
void print_snum(snum *value) {
	while (value != NULL) {
		printf("%d", (*value).digit);
		value = (*value).next;
	}
	printf("\n");
	return;
}

// A subtração e a divisão podem resultar em números com menos dígitos que o original. Esta função desaloca os dígitos vazios.
snum *remove_leading_zeros(snum *val) {
	if ((*val).digit != 0)
		return val; // Não altera nada se não há mais zeros à esquerda.
	else {
		if ((*val).next == NULL)
			return val; // Ocorre apenas se val continha apenas o dígito 0.
		else {
			snum *next_digit = (*val).next; 
			free(val);
			return remove_leading_zeros(next_digit);
		}
	}
}

/*****************************************************************************
*  Desafio: Inversão recursiva dos nódulos de uma lista ligada de tamanho n.
*  A função recursivamente percorre a lista original até o último nódulo,
*  nódulo este que se tornará o primeiro da lista invertida. A função então
*  devolve, a cada etapa, um nódulo com referência a este primeiro, visto que
*  ele será o nódulo retornado ao final de todo o processo.
*****************************************************************************/
snum *inverter_recursivo(const snum *original, int topo) {
  snum *nodulo = create_snum();
  (*nodulo).digit = (*original).digit;

  if ((*original).next == NULL) {
  	if (topo)
  		return nodulo; // Retorno final de todo o processo (n = 1).
  	else {
 	   (*nodulo).next = nodulo; // Este será o primeiro nódulo da lista invertida.
    	return nodulo;
    }
  } else {
    snum *retorno = inverter_recursivo((*original).next, 0);
    snum *primeiro = (*retorno).next; // Preservação da referência ao primeiro nódulo.
    (*retorno).next = nodulo;
    if (topo)
      return primeiro; // Retorno final de todo o processo (n > 1).
    else {
      (*nodulo).next = primeiro;
      return nodulo;
    }
  }
}

// Implementa recursivamene a adição de supernúmeros.
snum *superadd(snum *val1, snum *val2, int carry) {
	if (val1 == NULL && val2 == NULL) {
		// Trecho executado sse todos os dígitos foram somados.
		if (carry == 0) {
			return NULL;
		} else {
			snum *res = create_snum();
			(*res).digit = carry;
			return res;
		}
	} else {
		// Trecho executado sse ainda há dígitos a serem somados.
		int soma;
		snum *res = create_snum();
		if (val1 == NULL)
			soma = (*val2).digit;
		else if (val2 == NULL)
			soma = (*val1).digit;
		else
			soma = (*val1).digit + (*val2).digit;
		
		// Se computamos soma = 9+9 = 18, p. ex., o dígito atual será 8, e o dígito excedente (1) será transportado ('carried') para acréscimo no cômputo do próximo dígito.
		soma += carry;
		carry = 0;
		if (soma < 10)
			(*res).digit = soma;
		else {
			(*res).digit = soma - 10;
			carry = 1;
		}
		
		// Concatena o dígito atual ao cômputo do próximo, carregando para tal cômputo o excedente (>9) do cômputo do dígito atual.
		if (val1 == NULL)
			(*res).next = superadd(NULL, (*val2).next, carry);
		else if (val2 == NULL)
			(*res).next = superadd((*val1).next, NULL, carry);
		else
			(*res).next = superadd((*val1).next, (*val2).next, carry);
		
		// O supernúmero retornado é invertido em relação ao que será, em última instância, impresso na tela.
		return res;
	}
}

// Implementa recursivamene a multiplicação de supernúmeros.
snum *supermult(snum *val1, snum *val2) {
	// Se multiplicamos 123 por 456, realizaremos as multiplicações em partes: mutiplicaremos 123 por 4, então 123 por 5*10, então 123 por 6*10*10.
	// Os resultados parciais (123*4, 123*5, 123*6) serão obtidos por aux_supermult(), então serão corrigidos por ordem de grandeza (5 => 5*10, 6 => 6*100).
	// Os resultados corrigidos serão armazenados em uma supermatriz, e apenas então somados via superadd().
	smatrix *matriz_res = create_smatrix();
	smatrix *res_atual = matriz_res;
	snum *radical;
	int mult = 0;

	while (val2 != NULL) {
		// Computa o resultado parcial não-corrigido por ordem de grandeza.
		radical = aux_supermult(val1, val2, 0);
		free_snum((*res_atual).snumber);

		// Corrige os resultados parciais por ordem de grandeza.
		if (mult == 0)
			(*res_atual).snumber = radical;
		else {
			snum *zeros = create_snum();
			snum *ptr = zeros;
			for (int i = mult - 1; i > 0; i--) {
				(*ptr).next = create_snum();
				ptr = (*ptr).next;
			}
			(*ptr).next = radical;
			(*res_atual).snumber = zeros;
		}

		// Dá prosseguimento ao cômputo do próximo resultado parcial.
		val2 = (*val2).next;
		if (val2 != NULL) {
			(*res_atual).next = create_smatrix();
			res_atual = (*res_atual).next;
			mult++;
		}
	}

	// Adiciona os resultados parciais via superadd().
	res_atual = matriz_res;
	snum *res_inv = create_snum();
	while (res_atual != NULL) {
		snum *copy = res_inv;
		res_inv = superadd(copy, (*res_atual).snumber, 0);
		free_snum(copy);
		res_atual = (*res_atual).next;
	}
	free_smatrix(matriz_res);

	// O supernúmero retornado é invertido em relação ao que será, em última instância, impresso na tela.
	return res_inv;
}

// Computa recursiva os resultados parciais da multiplicação, sem correção por ordem de grandeza. Um dígito de val2 é empregado a cada uso da função.
snum *aux_supermult(snum *val1, snum *val2, int carry) {
	if (val1 == NULL) {
		// Trecho executado sse todos os dígitos de val1 já foram multiplicados pelo dígito (fixo) de val2.
		snum *res = create_snum();
		(*res).digit = carry;
		return res;
	} else {
		// Trecho executado sse ainda há dígitos de val1 a multiplicar.
		int prod = (*val1).digit * (*val2).digit;
		snum *res = create_snum();

		// Se multiplicamos prod = 9*9 = 81, o dígito atual será 1, e o excedente (8) será transportado ('carried') para acréscimo no cômputo do próximo dígito.
		prod += carry;
		carry = 0;
		if (prod < 10)
			(*res).digit = prod;
		else {
			(*res).digit = prod % 10;
			carry = prod / 10;
		}

		// Concatena o dígito atual ao cômputo do próximo, carregando para tal cômputo o excedente (>9) do cômputo do dígito atual.
		(*res).next = aux_supermult((*val1).next, val2, carry);
		return res;
	}
}

// Implementa recursivamene a divisão inteira de supernúmeros.
snum *superdiv(snum *numerator, snum *denominator, snum *quotient) {
	if ((*numerator).digit == 0) {
		// Tratamento caso o numerador seja iniciado pelo dígito 0.
		append_snum(quotient, create_snum());
		if ((*numerator).next == NULL) {
			// Trecho executado sse o numerador era tão somente o dígito 0. Termina o cálculo. Retorna o quociente recebido na função, só que com um 0 a mais no fim.
			free(numerator);
			return quotient;
		} else {
			 // Remove um zero que prefacia o numerador.
			snum *copy = numerator;
			numerator = (*numerator).next;
			free(copy);
			snum *new_numerator = copy_snum(numerator);
			free_snum(numerator);
			return superdiv(new_numerator, denominator, quotient);
		}

	} else {
		// Tratamento caso o numerador NÃO seja iniciado pelo dígito 0. Implementa uma divisão parcial pelo algoritmo usualmente ensinado no ensino básico do Brasil.

		// Armazena em 'remainder' a menor sequência de dígitos do numerador que é maior do que o denominador, em vistas de uma divisão parcial.
		snum *remainder = create_snum();
		snum *ptr_rem = remainder;
		(*remainder).digit = (*numerator).digit;
		snum *copy = numerator;
		numerator = (*numerator).next;
		free(copy);
		int num_zeros = -1;
		while (numerator != NULL && greater_than(denominator, remainder) > 0) {
			(*ptr_rem).next = create_snum();
			ptr_rem = (*ptr_rem).next;
			(*ptr_rem).digit = (*numerator).digit;
			copy = numerator;
			numerator = (*numerator).next;
			free(copy);
			num_zeros++;
		}
		
		// Realizando até nove operações repetidas de supersub(), calcula o quociente e o resto ('remainder') da divisão parcial.
		snum *quotient_digit = create_snum();
		while (greater_than(remainder, denominator) >= 0) {
			snum *val1 = inverter_recursivo(remainder, 1);
			snum *val2 = inverter_recursivo(denominator, 1);
			snum *sub_inv = supersub(val1, val2, 0);
			snum *sub = inverter_recursivo(sub_inv, 1);
			sub = remove_leading_zeros(sub);
			free_snum(val1);
			free_snum(val2);
			free_snum(sub_inv);
			free_snum(remainder);
			remainder = sub;
			(*quotient_digit).digit++;
		}
		
		// Calcula o novo quociente, baseado no número de subtrações realizadas (armazenado em quotient_digit).
		append_snum(quotient, quotient_digit);

		if (numerator == NULL) {
			// Termina o cálculo.
			free_snum(remainder);
			return quotient;
		} else {
			// Cálculo do próximo numerador. "Desce" os dígitos remanescentes do numerador, precedidos do resto da divisão parcial anterior caso haja resto.
			snum *new_numerator;
			if ((*remainder).digit == 0) {
				// Sendo o resto igual a 0, o próximo numerador é o que sobrou do numerador anterior. (O numerador "desce")
				new_numerator = copy_snum(numerator);
				free_snum(numerator);
				free_snum(remainder);
			} else {
				// Sendo o resto distinto de 0, o próximo numerador é o numerador anterior *precedido* do resto. (O numerador "desce")
				snum *numerator_leftover = copy_snum(numerator);
				free_snum(numerator);
				append_snum(remainder, numerator_leftover);
				new_numerator = remainder;
			}
			return superdiv(new_numerator, denominator, quotient);
		}
	}
}

// Implementa recursivamene a subtração em módulo de supernúmeros. O supernúmero val1 é assumido maior que o supernúmero val2.
snum *supersub(snum *val1, snum *val2, int carry) {
	if (val1 == NULL && val2 == NULL) {
		// Trecho executado sse todos os dígitos foram subtraídos.
		if (carry == 0) {
			return NULL;
		} else {
			snum *res = create_snum();
			(*res).digit = carry;
			return res;
		}
	} else {
		// Trecho executado sse ainda há dígitos a serem subtraídos.
		int sub;
		snum *res = create_snum();
		if (val1 == NULL)
			sub = 0 - (*val2).digit;
		else if (val2 == NULL)
			sub = (*val1).digit;
		else
			sub = (*val1).digit - (*val2).digit;
		// Se computamos sub = 1-9, p. ex., obtemos -8. Se obtemos sub < 0, emprestamos +10 do próximo dígito, obtendo o dígito atual (-8)+10 = 2.
		// A informação de que deve-se descontar 10 do cômputo do próximo dígito será transportada ('carried') para tal cômputo.
		sub -= carry;
		carry = 0;
		if (sub >= 0)
			(*res).digit = sub;
		else {
			(*res).digit = 10 + sub;
			carry = 1;
		}
		// Concatena o dígito atual ao cômputo do próximo, carregando para tal cômputo a informação de se deve-se descontar 10 de tal cômputo.
		if (val1 == NULL)
			(*res).next = supersub(NULL, (*val2).next, carry);
		else if (val2 == NULL)
			(*res).next = supersub((*val1).next, NULL, carry);
		else
			(*res).next = supersub((*val1).next, (*val2).next, carry);
		// O supernúmero retornado é invertido em relação ao que será, em última instância, impresso na tela.
		return res;
	}
}

// Retorna 1 se val1 > val2, 0 se val1 == val2, e -1 se val1 < val2.
int greater_than(snum *val1, snum *val2) {
	int result_digits = more_digits_than(val1, val2);
	if (result_digits != 0)
		// Se val1 e val2 têm diferentes quantias de dígitos, more_digits_than() diz qual o maior.
		return result_digits;
	else  {
		// Se val1 e val2 possuem o mesmo número de dígitos, aux_greater_than() diz qual o maior.
		return aux_greater_than(val1, val2);
	}
}

// Determina recursivamente se possui mais dígitos val1 (retorno 1), val2 (retorno -1), ou nenhum (retorno 0).
int more_digits_than(snum *val1, snum *val2) {
	// A função percorre um-a-um os dígitos dos supernúmeros em paralelo e determina qual termina primeiro.
	if (val1 != NULL && val2 != NULL)
		return more_digits_than((*val1).next, (*val2).next);
	else if (val1 == NULL && val2 != NULL)
		return -1;
	else if (val1 != NULL && val2 == NULL)
		return 1;
	else
		return 0;
}

// Determina recursivamente se val1 > val2 (retorno 1), val1 == val2 (retorno 0), ou val1 < val2 (retorno -1).
int aux_greater_than(snum *val1, snum *val2) {
	// A função assume que val1 e val2 possuem o mesmo número de dígitos. O algoritmo consiste em comparar os dígitos um-a-um.
	// Assim que um dígito for maior que o seu correspondente no outro supernúmero, sabemos que o dono do primeiro dígito é maior.
	if (val1 == NULL && val2 == NULL)
		return 0;
	else if ((*val1).digit > (*val2).digit)
		return 1;
	else if ((*val1).digit < (*val2).digit)
		return -1;
	else
		return aux_greater_than((*val1).next, (*val2).next);
}