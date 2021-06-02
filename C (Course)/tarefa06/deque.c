#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deque.h"

// Aloca dinamicamente uma lista duplamente ligada.
DLL *criar_DLL(void) {
	DLL *dll = malloc(sizeof(DLL));
	(*dll).val = 0;
	(*dll).prev = NULL;
	(*dll).next = NULL;
	return dll;
}

// Desaloca recursivamente uma lista duplamente ligada dinamicamente alocada.
void free_DLL(DLL *dll) {
	if (dll != NULL) {
		if ((*dll).next != NULL)
			free_DLL((*dll).next);
		free(dll);
	}
	return;
}

// Via recursão, aloca dinamicamente uma lista duplamente ligada com a mesma sequência de valores que a original.
DLL *copiar_DLL(DLL *prev, DLL *dll) {
	if (dll == NULL)
		return NULL;
	else {
		DLL *new = criar_DLL();
		(*new).val = (*dll).val;
		(*new).prev = prev;
		(*new).next = copiar_DLL(new, (*dll).next);
		return new;
	}
	return dll;
}

// Aloca dinamicamente um paciente com ID único, determinado pela ordem de entrada. Possui estrutura de lista duplamente ligada.
pac *criar_pac(int id) {
	pac *paciente = malloc(sizeof(pac));
	(*paciente).id = id;
	(*paciente).nome = NULL;
	(*paciente).prioridade = NULL;
	(*paciente).atendimentos = NULL;
	(*paciente).prev = NULL;
	(*paciente).next = NULL;
	return paciente;
}

// Desaloca recursivamente um paciente dinamicamente alocado, que possui estrutura de lista duplamente ligada.
void free_pac(pac *paciente) {
	if (paciente != NULL) {
		free((*paciente).nome);
		free((*paciente).prioridade);
		free_DLL((*paciente).atendimentos);
		if ((*paciente).next != NULL)
			free_pac((*paciente).next);
		free(paciente);
	}
	return;
}

// Aloca dinamicamente um paciente com o mesmo nome, prioridade, e lista ligada de atendimentos que o original.
// Isso é feito sem copiar suas ligações com outros pacientes; por isso "isolado".
pac *copiar_pac_isolado(pac *paciente) {
	if (paciente == NULL)
		return NULL;
	else {
		pac *new = criar_pac((*paciente).id);
		char *nome = malloc(101 * sizeof(char));
		strcpy(nome, (*paciente).nome);
		(*new).nome = nome;
		char *prioridade = malloc(13 * sizeof(char));
		strcpy(prioridade, (*paciente).prioridade);
		(*new).prioridade = prioridade;
		(*new).atendimentos = copiar_DLL(NULL, (*paciente).atendimentos);
		return new;
	}
}

// Via recursão, tenta encontrar um paciente pelo seu ID (único para cada paciente), e então o copia via copiar_pac_isolado().
pac *copiar_pac_isolado_id(pac *paciente, int id) {
	if (paciente == NULL)
		return NULL;
	else if ((*paciente).id == id)
		return copiar_pac_isolado(paciente);
	else
		return copiar_pac_isolado_id((*paciente).next, id);
}

// Aloca dinamicamente uma fila com entrada e saída tanto no fim quanto no começo (double-ended queue ou "deque").
dq *criar_deque(void) {
	dq *deque = malloc(sizeof(dq));
	(*deque).ini = NULL;
	(*deque).fim = NULL;
	return deque;
}

// Via recursão, desaloca uma "deque" dinamicamente alocada.
void free_deque(dq *deque) {
	if (deque != NULL) {
		if ((*deque).ini != NULL) {
			remove_inicio(deque);
			free_deque(deque);
		} else
			free(deque);
	}
	return;
}

// Insere um paciente no início de uma "deque", desfazendo suas ligações anteriores e fazendo ligações novas.
void insere_inicio(dq *deque, pac *paciente) {
	(*paciente).prev = NULL;
	if ((*deque).ini == NULL) {
		(*paciente).next = NULL;
		(*deque).ini = paciente;
		(*deque).fim = paciente;
	} else {
		(*paciente).next = (*deque).ini;
		(*deque).ini->prev = paciente;
		(*deque).ini = paciente;
	}
	return;
}

// Insere um paciente no final de uma "deque", desfazendo suas ligações anteriores e fazendo ligações novas.
void insere_fim(dq *deque, pac *paciente) {
	(*paciente).next = NULL;
	if ((*deque).ini == NULL) {
		(*paciente).prev = NULL;
		(*deque).ini = paciente;
		(*deque).fim = paciente;
	} else {
		(*paciente).prev = (*deque).fim;
		(*deque).fim->next = paciente;
		(*deque).fim = paciente;
	}
	return;
}

// Remove o paciente na frente de uma "deque", também o desalocando via free_pac().
void remove_inicio(dq *deque) {
	if ((*deque).ini != NULL) {
		if ((*deque).ini->next == NULL) {
			free_pac((*deque).ini);
			(*deque).ini = NULL;
			(*deque).fim = NULL;
		} else {
			pac *inicio = (*deque).ini;
			(*deque).ini = (*deque).ini->next;
			(*deque).ini->prev = NULL;
			(*inicio).next = NULL;
			free_pac(inicio);
		}
	}
	return;
}

// Remove o paciente no fim de uma "deque", também o desalocando via free_pac().
void remove_fim(dq *deque) {
	if ((*deque).fim != NULL) {
		if ((*deque).fim->prev == NULL) {
			free_pac((*deque).fim);
			(*deque).ini = NULL;
			(*deque).fim = NULL;
		} else {
			pac *final = (*deque).fim;
			(*deque).fim = (*deque).fim->prev;
			(*deque).fim->next = NULL;
			free_pac(final);
		}
	}
	return;
}