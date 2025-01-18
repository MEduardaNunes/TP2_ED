#ifndef UNIDADE_H
#define UNIDADE_H

#include <time.h>


/*
 * Estrutura de uma Unidade
 *
 * Armazena as informações de uma Unidade de um Procedimento,
 * sendo elas o tempo ocioso, a última data de atualização
 * (quando foi desocupada ou ocupada) e se está ocupada ou não.
*/
typedef struct {
    int ocupado;
    double tempoOcioso;
    time_t ultimaData;
} Unidade;


Unidade inicializaUnidade();

#endif