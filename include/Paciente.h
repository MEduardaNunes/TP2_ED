#ifndef PACIENTE_H
#define PACIENTE_H

#include <time.h>


/*
 * Estrutura de um Paciente
 *
 * Armazena as informações de um Paciente do hospital, que são
 * identificador do paciente, se teve alta ou não, as informações
 * da data de adimissão (ano, mês, dia e hora), o grau de urgência,
 * a quantidade de medidas hospitalares, de testes de laboratório,
 * de exames de imagem e de instrumentos/medicamentos, o estado
 * do Paciente, o id da Unidade ocupada, dataFim e os horários de
 * tempo em que o Paciente esteve ocioso e em atendimento.
*/
typedef struct {
    //prontuario do paciente
    float id;
    int alta;
    time_t dataAdmissao;
    int grauUrgencia;
    int quantidades[4];

    //estado e estatisticas
    int estado;
    int idUnidade;
    time_t dataFim;
    double tempoAtendido;
    double tempoOcioso;
} Paciente;


Paciente* inicializaPaciente(float id, int alta, int ano, int mes, int dia, float hora, int gu, int qMH, int qTL, int qEI, int qIM);
int pacienteMenor(Paciente *p1, Paciente *p2);
double determinaQuantidade(Paciente *p);
void imprimePaciente(Paciente *p);

#endif