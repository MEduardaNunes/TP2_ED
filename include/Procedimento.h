#ifndef PROCEDIMENTO_H
#define PROCEDIMENTO_H

#include "Unidade.h"

#include <time.h>


/*
 * Estrutura de um Procedimento
 *
 * Armazena as informações de um Procedimento do hospital,
 * sendo elas o tempo de atendimento médio, a quantidade
 * de atendentes e as unidades.
*/
typedef struct {
    double tempo;
    int qntdAtendentes;
    Unidade *unidades;
} Procedimento;


Procedimento* inicializaProcedimento(double tempo, int qA);
void inicializaHora(Procedimento *p, time_t horaInicial);
int achaUnidade(Procedimento *p);
int procedimentoOcupado(Procedimento *p);
void finalizaHora(Procedimento *p, time_t horaFinal);
void finalizaProcedimentos(Procedimento **p);

#endif