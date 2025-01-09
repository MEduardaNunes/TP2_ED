#include "../include/Paciente.h"
#include "../include/Data.h"
#include "../include/msgassert.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * Variáveis globais para melhoria da legibilidade do código. Elas são:
 * MH - Medidas hospitalares
 * TL - Testes de Laboratório
 * EI - Exames de imagem
 * IM - Instrumentos/medicamentos
 * Utilizadas para acessar os índices do atributo 'quantidades'
 * de pacientes.
 */
#define MH 0
#define TL 1
#define EI 2
#define IM 3

/*
 * \brief Construtor de Paciente
 *
 * Inicializa um novo Paciente com as informações lidas no arquivo
 * de entrada, além de iniciliazar o estado, as datas e o idUnidade.
 * 
 * \param id O identificador do paciente.
 * \param alta Se o paciente teve alta ou não após atendimento (0 ou 1).
 * \param ano O ano da chegada do paciente no hospital.
 * \param mes O mês da chegada do paciente no hospital.
 * \param dia O dia da chegada do paciente no hospital.
 * \param hora A hora da chegada do paciente no hospital.
 * \param gu O grau de urgência do paciente.
 * \param qMH A quantidade de medidas hospitalares realizadas.
 * \param qTL A quantidade de testes de laboratórios realizados.
 * \param qEI A quantidade de exames de imagem realizados.
 * \param qIM A quantidade de instrumentos/medicamentos utilizados.
 * \return O novo Paciente.
 */
Paciente* inicializaPaciente(float id, int alta, int ano, int mes, int dia, float hora, int gu, int qMH, int qTL, int qEI, int qIM) {
    Paciente *novo_paciente = (Paciente*) malloc(sizeof(Paciente));
    erroAssert(novo_paciente, "Memoria insuficiente");

    novo_paciente->id = id;
    novo_paciente->alta = alta;
    novo_paciente->dataAdmissao = inicializaData(ano, mes, dia, hora);
    novo_paciente->grauUrgencia = gu;
    novo_paciente->quantidades[MH] = qMH;
    novo_paciente->quantidades[TL] = qTL;
    novo_paciente->quantidades[EI] = qEI;
    novo_paciente->quantidades[IM] = qIM;

    novo_paciente->estado = 1;
    novo_paciente->idUnidade = -1;
    novo_paciente->dataInicio = inicializaData(ano, mes, dia, hora);
    novo_paciente->dataFim = inicializaData(ano, mes, dia, hora);
    novo_paciente->tempoOcioso = 0.0;
    novo_paciente->tempoAtendido = 0.0;

    return novo_paciente;
}


/*
 * \brief Compara dois pacientes
 *
 * Essa função recebe dois Pacientes e compara a data final
 * de ambos pacientes. Caso elas sejam iguais, é comparado
 * os ids dos pacientes. As operações de comparação são apenas
 * '<' e '>'. A função retorna 1 caso a comparação tenha
 * sucesso, ou 0 caso contrário.
 * 
 * \param p1 O primeiro Paciente.
 * \param p2 O segundo Paciente.
 * \param op A operação de comparação dos Pacientes.
 * \return O resultado da comparação dos pacientes (0 ou 1).
*/
int comparaPacientes(Paciente *p1, Paciente *p2, char *op) {
    if((strcmp("<", op) && strcmp(">", op)) || !p1 || !p2) {
        avisoAssert(!strcmp("<", op) || !strcmp(">", op), "Operacao invalida.");
        avisoAssert(p1, "Paciente nulo.");
        avisoAssert(p2, "Paciente nulo.");
        return -1;
    }

    if (!strcmp("<", op)) {
        if (dataIgual (p1->dataFim, p2->dataFim)) return p1->id < p2->id;
        return dataMenor(p1->dataFim, p2->dataFim);
    }
    
    if (!strcmp(">", op)) {
        if (dataIgual(p1->dataFim, p2->dataFim)) return p1->id > p2->id;
        return dataMenor(p2->dataFim, p1->dataFim);
    }
}


/*
 * \brief Atualiza o tempo atendido de um Paciente
 *
 * Essa função recebe um Paciente e uma data, e 
 * atualiza seu tempo atendido, bem como a data de 
 * início para o horário atual.
 * 
 * \param p O Paciente a ser atualizado.
 * \param horarioAtual A data do horário da atualização.
*/
void atualizaAtePaciente(Paciente *p, Data horarioAtual) {
    if (!p) {
        avisoAssert(p, "Paciente nulo.");
        return;
    }

    //atualizando tempo
    float tempo = subtraiData(horarioAtual, p->dataInicio);
    p->tempoAtendido += tempo;

    //printf("ATE %.0f %d %d %.2f ", p->id, p->grauUrgencia, p->estado, tempo);
    //imprimeData(horarioAtual);
    //imprimeData(p->dataFim);
    //printf("\n");

    //atualizando data
    copiaData(&p->dataInicio, horarioAtual);
}


/*
 * \brief Atualiza o tempo ocioso de um Paciente
 *
 * Essa função recebe um Paciente e uma data, e 
 * atualiza seu tempo ocioso, bem como a data de 
 * fim para o horário atual.
 * 
 * \param p O Paciente a ser atualizado.
 * \param horarioAtual A data do horário da atualização.
*/
void atualizaOciPaciente(Paciente *p, Data horarioAtual) {
    if (!p) {
        avisoAssert(p, "Paciente nulo.");
        return;
    }

    //atualizando tempo
    float tempo = subtraiData(horarioAtual, p->dataFim);
    p->tempoOcioso += tempo;

    //printf("OCI %.0f %d %d %.2f ", p->id, p->grauUrgencia, p->estado, tempo);
    //imprimeData(p->dataFim);
    //imprimeData(horarioAtual);
    //printf("\n");

    //atualizando data
    copiaData(&p->dataFim, horarioAtual);
}


/*
 * \brief Imprime um Paciente
 *
 * Essa função recebe um Paciente e imprime suas informações
 * no terminal no formato especificado.
 * 
 * \param p O Paciente a ser impresso.
*/
void imprimePaciente(Paciente *p) {
    if (!p) {
        avisoAssert(p, "Paciente nulo.");
        return;
    }

    printf("%.0f ", p->id);
    imprimeData(p->dataAdmissao);
    imprimeData(p->dataFim);
    printf("%.2f %.2f %.2f\n", p->tempoAtendido + p->tempoOcioso, p->tempoAtendido, p->tempoOcioso);
}