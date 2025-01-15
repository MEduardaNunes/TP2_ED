#include "../include/Paciente.h"
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

    //data para unix
    int hora_i = (int) hora;
    int minutos_i = (hora - hora_i) * 60;

    struct tm data = {0};
    data.tm_year = ano - 1900;
    data.tm_mon = mes - 1;
    data.tm_mday = dia;
    data.tm_hour = hora_i;
    data.tm_min = minutos_i;

    time_t data_inicial = mktime(&data);

    novo_paciente->id = id;
    novo_paciente->alta = alta;
    novo_paciente->dataAdmissao = data_inicial;
    novo_paciente->grauUrgencia = gu;
    novo_paciente->quantidades[MH] = qMH;
    novo_paciente->quantidades[TL] = qTL;
    novo_paciente->quantidades[EI] = qEI;
    novo_paciente->quantidades[IM] = qIM;

    novo_paciente->estado = 1;
    novo_paciente->dataFim = data_inicial;
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
        if (p1->dataFim == p2->dataFim) return p1->id < p2->id;
        return p1->dataFim < p2->dataFim;
    }
    
    if (!strcmp(">", op)) {
        if (p1->dataFim == p2->dataFim) return p1->id > p2->id;
        return p1->dataFim > p2->dataFim;
    }
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

    char *data_inicio = ctime(&p->dataAdmissao);
    if (data_inicio[strlen(data_inicio) - 1] == '\n') data_inicio[strlen(data_inicio) - 1] = '\0';
    printf("%.0f %s ", p->id, data_inicio);

    char *data_fim = ctime(&p->dataFim);
    if (data_fim[strlen(data_fim) - 1] == '\n') data_fim[strlen(data_fim) - 1] = '\0';

    printf("%s %.2f %.2f %.2f\n", data_fim, p->tempoAtendido + p->tempoOcioso, p->tempoAtendido, p->tempoOcioso);
}