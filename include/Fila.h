#ifndef FILA_H
#define FILA_H

#include "Paciente.h"


/*
 * Estrutura de uma Fila
 *
 * Armazena as informações da fila de Pacientes, sendo elas
 * frente, tras e tamanho máximo.
*/
typedef struct {
    int frente;
    int tras;
    int tamMax;
    int qntd;
    Paciente **pacientes;
} Fila;


Fila** inicializaFilas(int qntdMax);
void enfileira(Fila *f, Paciente *p);
Paciente* desinfileira(Fila *f);
int filaVazia(Fila *f);
void finalizaFilas(Fila **f);

#endif