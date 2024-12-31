#ifndef DATA_H
#define DATA_H


/*
 * Estrutura de uma Data
 *
 * Armazena as informações de uma Data, sendo elas
 * ano, mês dia e hora.
*/
typedef struct {
    int ano;
    int mes;
    int dia;
    float hora;
} Data;


Data inicializaData(int a, int m, int d, float h);
Data somaData(Data d, float horas);
float subtraiData(Data dMaior, Data dMenor);
void copiaData(Data *destino, Data origem);
int dataMenor(Data dMenor, Data dMaior);
int dataIgual(Data d1, Data d2);
void imprimeData(Data d);

#endif