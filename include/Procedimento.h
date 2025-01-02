#ifndef PROCEDIMENTO_H
#define PROCEDIMENTO_H

#include "Data.h"
#include "Unidade.h"

/*
 * Estrutura de um Procedimento
 *
 * Armazena as informações de um Procedimento do hospital,
 * sendo elas o tempo de atendimento médio, a quantidade
 * de atendentes e as unidades.
*/
typedef struct {
    float tempo;
    int qntdAtendentes;
    Unidade *unidades;
} Procedimento;


Procedimento* inicializaProcedimento(float tempo, int qA);
int achaUnidadeVazia(Procedimento *p);
int procedimentoOcupado(Procedimento *p);
void ocupaUnidade(Procedimento *p, int unidade, Data horario);
void atualizaEstProcedimentos(Procedimento **p, Data horarioAtual);
void finalizaProcedimentos(Procedimento **p);

#endif