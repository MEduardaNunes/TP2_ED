#include "../include/msgassert.h"
#include "../include/Procedimento.h"


#define QPROC 6


/*
 * \brief Construtor de Procedimento
 *
 * Inicializa um novo Procedimento com as informações lidas
 * no arquivo de entrada, como o tempo médio e a quantidade
 * de atendentes doprocedimento, além de inicializar as 
 * unidades do Procedimento.
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
    novo_procedimento->unidades = (int*) calloc(qA, sizeof(int));
    erroAssert(novo_procedimento->unidades, "Memoria insuficiente"); 

    return novo_procedimento;
}


/*
 * \brief Acha a primeira unidade vazia de um Procedimento
 *
 * Essa função faz uma busca sequencial nas unidades do Procedimento
 * e retorna o id da primeira Unidade encontrada que está vazia.
 * Caso não encontre, é retornado -1.
 * 
 * \param p O procedimento.
 * \return O id da unidade vazia ou -1.
 */
int achaUnidadeVazia(Procedimento *p) {
    if (!p) {
        avisoAssert(p, "Procedimento nulo.");
        return -1;
    }

    int unidade = -1;
    for (int i = 0; i < p->qntdAtendentes; i++) {
        if(!p->unidades[i]) {
            unidade = i;
            break;
        }
    }

    return unidade;
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

    return (achaUnidadeVazia(p) == -1 ? 1 : 0);
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