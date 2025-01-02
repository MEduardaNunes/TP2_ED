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
    novo_procedimento->unidades = (Unidade*) malloc(sizeof(Unidade) * qA);
    erroAssert(novo_procedimento->unidades, "Memoria insuficiente"); 
    for (int i = 0; i < qA; i++) novo_procedimento->unidades[i] = inicializaUnidade();

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
        if(!p->unidades[i].ocupado) {
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
 * \brief Ocupa uma Unidade de um Procedimento
 *
 * Essa função recebe um Procedimento, um id de uma Unidade e uma Data,
 * então é ocupada a Unidade do id recebido do Procedimento e atualizado
 * o tempo ocioso ou de atendimento da Unidade.
 * 
 * \param p O procedimento a ser ocupado.
 * \param unidade O id da unidade a ser ocupada.
 * \param horario A Data da ocupação.
 */
void ocupaUnidade(Procedimento *p, int unidade, Data horario) {
    if (!p || (unidade < 0 || unidade >= p->qntdAtendentes)) {
        avisoAssert(p, "Procedimento nulo.");
        avisoAssert(unidade >= 0 && unidade < p->qntdAtendentes, "Id inválido.");
        return;
    }

    if (p->unidades[unidade].ocupado) atualizaAteUnidade(&p->unidades[unidade], horario);
    else atualizaOciUnidade(&p->unidades[unidade], horario);
    p->unidades[unidade].ocupado = 1;
}


/*
 * \brief Atualiza as estatísticas dos Procedimentos
 *
 * Essa função recebe os Procedimentos e uma data e percorre
 * sequencialmente as Unidades, atualizando o tempo de atendimento
 * ou ocioso, caso esteja ocupado ou não, respectivamente.
 * 
 * \param p Os procedimentos a serem atualizados.
 * \param horarioAtual A Data da atualização.
 */
void atualizaEstProcedimentos(Procedimento **p, Data horarioAtual) {
    if (!p) {
        avisoAssert(p, "Procedimento nulo.");
        return;
    }

    for (int i = 0; i < QPROC; i++) {
        for (int j = 0; j < p[i]->qntdAtendentes; j++) {
            if (p[i]->unidades[j].ocupado) atualizaAteUnidade(&p[i]->unidades[j], horarioAtual);
            else atualizaOciUnidade(&p[i]->unidades[j], horarioAtual);
        }
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