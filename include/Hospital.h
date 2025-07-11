#ifndef HOSPITAL_H
#define HOSPITAL_H

#include "Escalonador.h"
#include "Fila.h"
#include "Paciente.h"
#include "Procedimento.h"

#include <time.h>


/*
 * Estrutura de um Hospital
 *
 * Contém as informações do hospital sendo elas:
 * - O relógio do hospital
 * - Os pacientes lidos do arquivo
 * - As filas de cada procedimento e de cada urgência,
 * ou seja, cada índice da fila corresponde à urgência,
 * sendo 0 - Verde, 1 - Amarelo e 2 - Vermelho. Apenas a
 * fila da triagem é única.
 * - Os procedimentos do hospital: triagem, atendimento,
 * testes de laboratório, exames de imagem e instrumentos/
 * medicamentos (índices 0 a 5, respectivamente).
 * - O escalonador do hospital, que controla a simulação.
*/
typedef struct {
    //relogio global
    time_t relogioHospital;

    //pacientes lidos
    int qntdPacientes;
    Paciente **pacientesHospital;

    //filas, procedimentos e escalonador do hospital
    Fila **filas;
    Procedimento **procedimentos;
    Escalonador escalonadorHospital;
} Hospital;


Hospital* preencheHospital(char *nomeArq);
Procedimento* determinaProcedimento(Paciente *p, Hospital *hospital);
void mudaEstado(Paciente *p, Hospital *hospital);
void moveParaFila(Paciente *p, Hospital *hospital);
void moveParaAtendimento(Hospital *hospital);
void verificaProcedimento(int id, Hospital *hospital);
void simulaHospital(Hospital *hospital);
void imprimeHospital(Hospital *hospital);
void finalizaHospital(Hospital *hospital);

#endif