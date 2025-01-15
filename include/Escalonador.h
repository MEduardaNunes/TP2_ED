#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "Paciente.h"

/*
 * Estrutura de um Escalonador
 *
 * Simula um minHeap em um vetor,
 * armazena a quantidade de pacientes, 
 * os pacientes e o tamanho máximo.
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
void finalizaEscalonador(Escalonador *e);


#endif