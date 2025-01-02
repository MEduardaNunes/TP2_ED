#ifndef UNIDADE_H
#define UNIDADE_H

#include "Data.h"


/*
 * Estrutura de uma Unidade
 *
 * Armazena as informações de uma unidade de um Procedimento,
 * sendo elas se a unidade está ocupada, os períodos de aten-
 * dimento da unidade e a última data de atualização da unidade.
*/
typedef struct {
    int ocupado;
    float tempoAtendimento;
    float tempoOcioso;
    Data ultimoHorario;
} Unidade;


Unidade inicializaUnidade();
void atualizaAteUnidade(Unidade *u, Data horarioAtual);
void atualizaOciUnidade(Unidade *u, Data horarioAtual);

#endif
