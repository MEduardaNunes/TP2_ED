#include "../include/msgassert.h"
#include "../include/Procedimento.h"


#define QPROC 6


/*
 * \brief Construtor de Procedimento
 *
 * Inicializa um novo Procedimento com as informações lidas
 * no arquivo de entrada, como o tempo médio e a quantidade
 * de atendentes do procedimento..
 * 
 * \param tempo O tempo médio do procedimento.
 * \param qA A quantidade de unidades disponíveis.
 * \return O novo Procedimento.
 */
Procedimento* inicializaProcedimento(float tempo, int qA) {
    if (tempo < 0 || qA < 0) {
        avisoAssert(tempo >= 0, "Tempo inválido.");
        avisoAssert(qA >= 0, "Quantidade inválida");
        return NULL;
    }

    Procedimento* novo_procedimento = (Procedimento*) malloc(sizeof(Procedimento));
    erroAssert(novo_procedimento, "Memoria insuficiente.");

    novo_procedimento->tempo = tempo;
    novo_procedimento->qntdAtendentes = qA;
    novo_procedimento->unidadesOcupadas = 0;

    return novo_procedimento;
}


/*
 * \brief Verifica se um Procedimento está ocupado
 *
 * Essa função faz uma busca sequencial nas unidades do Procedimento
 * e retorna se todas as unidades estão ocupadas. Caso todas estejam
 * ocupadas, retorna 1, e 0 caso contrário.
 * 
 * \param p O procedimento a ser verificado.
 * \return 0 ou 1.
 */
int procedimentoOcupado(Procedimento *p) {
    if (!p) {
        avisoAssert(p, "Procedimento nulo.");
        return -1;
    }

    return (p->qntdAtendentes == p->unidadesOcupadas);
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
        free(p[i]);
    }

    free(p);
}