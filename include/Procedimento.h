#ifndef PROCEDIMENTO_H
#define PROCEDIMENTO_H

/*
 * Estrutura de um Procedimento
 *
 * Armazena as informações de um Procedimento do hospital,
 * sendo elas o tempo de atendimento médio, a quantidade
 * de atendentes e a quantidade de unidades ocupadas.
*/
typedef struct {
    double tempo;
    int qntdAtendentes;
    int unidadesOcupadas;
} Procedimento;


Procedimento* inicializaProcedimento(double tempo, int qA);
int procedimentoOcupado(Procedimento *p);
void finalizaProcedimentos(Procedimento **p);

#endif