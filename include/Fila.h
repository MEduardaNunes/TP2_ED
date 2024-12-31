#ifndef FILA_H
#define FILA_H

#include "Paciente.h"


/*
 * Estrutura de uma Fila
 *
 * Armazena as informações da fila de Pacientes,
 * sendo elas frente, tras e tamanho máximo.
*/
typedef struct {
    int frente;
    int tras;
    int tamMax;
    int qntd;
    Paciente **pacientes;
} Fila;


Fila* inicializaFila(int qntdMax);
void enfileira(Fila *f, Paciente *p);
Paciente* desinfileira(Fila *f);
int filaVazia(Fila *f);
void atualizaEstFilas(Fila **f, Data horario);
void finalizaFila(Fila **f);

#endif