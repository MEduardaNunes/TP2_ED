#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "Data.h"
#include "Paciente.h"

/*
 * Estrutura de um Escalonador
 *
 * Armazena a raiz do minHeap, utilizado
 * para simular a fila de prioridade.
*/
typedef struct {
    int qntd;
    int tamMax;
    Paciente **pacientes;
} Escalonador;


Escalonador inicializaEscalonador(int qntdMax);
int getAncestral(int posicao);
int getSucessorEsq(int posicao);
int getSucessorDir(int posicao);
void insereEvento(Escalonador *e, Paciente *p);
Paciente* retiraProximoEvento(Escalonador *e);
int escalonadorVazio(Escalonador *e);
void atualizaEstEscalonador(Escalonador *e, Data horario);
void finalizaEscalonador(Escalonador *e);


#endif