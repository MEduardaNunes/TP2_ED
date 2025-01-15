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
 * do Paciente, dataFim e os horários de tempo em que o Paciente
 * esteve ocioso e em atendimento.
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
    time_t dataFim;
    float tempoAtendido;
    float tempoOcioso;
} Paciente;


Paciente* inicializaPaciente(float id, int alta, int ano, int mes, int dia, float hora, int gu, int qMH, int qTL, int qEI, int qIM);
int comparaPacientes(Paciente *p1, Paciente *p2, char *op);
time_t arredondaTime(time_t t);
void imprimePaciente(Paciente *p);
double determinaQuantidade(Paciente *p);

#endif