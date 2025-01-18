#include "../include/msgassert.h"
#include "../include/Procedimento.h"


#define QPROC 6


/*
 * \brief Construtor de Procedimento
 *
 * Inicializa um novo Procedimento com as informações lidas
 * no arquivo de entrada, como o tempo médio e a quantidade
 * de atendentes do procedimento, e inicializa as Unidades.
 * 
 * \param tempo O tempo médio do procedimento.
 * \param qA A quantidade de unidades disponíveis.
 * \return O novo Procedimento.
 */
Procedimento* inicializaProcedimento(double tempo, int qA) {
    if (tempo < 0 || qA < 0) {
        avisoAssert(tempo >= 0, "Tempo inválido.");
        avisoAssert(qA >= 0, "Quantidade inválida");
        return NULL;
    }

    Procedimento* novo_procedimento = (Procedimento*) malloc(sizeof(Procedimento));
    erroAssert(novo_procedimento, "Memoria insuficiente.");

    novo_procedimento->tempo = tempo;
    novo_procedimento->qntdAtendentes = qA;
    novo_procedimento->unidades = (Unidade*) malloc(sizeof(Unidade) * qA);
    erroAssert(novo_procedimento->unidades, "Memoria insuficiente.");
    for (int i = 0; i < qA; i++) novo_procedimento->unidades[i] = inicializaUnidade();

    return novo_procedimento;
}


/*
 * \brief Inicializa as horas das Unidades de um Procedimento
 *
 * Inicializa o atributo 'ultimaData' de todas as Unidades de 
 * um Procedimento com a hora inicial recebida.
 * 
 * \param p O Procedimento das unidades.
 * \param horaInicial A hora inicial.
 */
void inicializaHora(Procedimento *p, time_t horaInicial) {
    for (int i = 0; i < p->qntdAtendentes; i++) p->unidades[i].ultimaData = horaInicial;
}


/*
 * \brief Acha a primeira Unidade vazia
 *
 * Percorre sequencialmente as Unidades de um Procedimento
 * e retorna o id da primeira que estiver vazia. Caso todas
 * estejam ocupadas, retorna -1.
 * 
 * \param p O Procedimento das unidades.
 * \return O id da Unidade vazia, ou -1.
 */
int achaUnidade(Procedimento *p) {
    int id = -1;

    for (int i = 0; i < p->qntdAtendentes; i++) {
        if (!p->unidades[i].ocupado) {
            id = i;
            break;
        }
    }

    return id;
}


/*
 * \brief Verifica se um Procedimento está ocupado
 *
 * Essa função faz uma busca sequencial nas unidades do Procedimento
 * e retorna se todas as unidades estão ocupadas. Caso todas estejam
 * ocupadas, retorna 1, ou 0 caso contrário.
 * 
 * \param p O procedimento a ser verificado.
 * \return 0 ou 1.
 */
int procedimentoOcupado(Procedimento *p) {
    if (!p) {
        avisoAssert(p, "Procedimento nulo.");
        return -1;
    }

    return (achaUnidade(p) == -1 ? 1 : 0);
}


/*
 * \brief Finaliza as horas das Unidades de um Procedimento
 *
 * Faz a última atribuição do tempo ocioso de cada Unidade
 * de um Procedimento, além de atribuir a ultimaData a 
 * horaFinal recebida.
 * 
 * \param p O Procedimento das unidades.
 * \param horaFinal A hora final.
 */
void finalizaHora(Procedimento *p, time_t horaFinal) {
    for (int i = 0; i < p->qntdAtendentes; i++) {
        p->unidades[i].tempoOcioso += ((horaFinal - p->unidades[i].ultimaData) / 3600.0);
        p->unidades[i].ultimaData = horaFinal;
    }
}


/*
 * \brief Finalina os Procedimentos
 *
 * Essa função recebe os Procedimentos e desaloca as 
 * memórias alocadas dinamicamente dessas estruturas.
 * 
 * \param p Os Procedimentos a serem finalizados.
 */
void finalizaProcedimentos(Procedimento **p) {
    if (!p) {
        avisoAssert(p, "Procedimento nulo.");
        return;
    }

    for (int i = 0; i < QPROC; i++) {
        free(p[i]->unidades);
        free(p[i]);
        }

    free(p);
}