#include "../include/msgassert.h"
#include "../include/Unidade.h"


/*
 * \brief Inicializa uma Unidade
 *
 * Essa função inicializa uma nova Unidade com os valores
 * iniciais.
 * 
 * \return A nova Unidade.
 */
Unidade inicializaUnidade() {
    Unidade nova_unidade;
    Data horario_inicial = inicializaData(0, 0, 0, 0.0);

    nova_unidade.ocupado = 0;
    nova_unidade.tempoAtendimento = 0.0;
    nova_unidade.tempoOcioso = 0.0;
    copiaData(&nova_unidade.ultimoHorario, horario_inicial);

    return nova_unidade;
}


/*
 * \brief Atualiza o tempo de atendimento de uma Unidade
 *
 * Essa função recebe uma Unidade e uma data, e atualiza
 * seu tempo de atendimento, bem como o horarioAtual.
 * 
 * \param u A Unidade a ser atualizada.
 * \param horarioAtual A data do horário da atualização.
*/
void atualizaAteUnidade(Unidade *u, Data horarioAtual) {
    if (!u) {
        avisoAssert(u, "Unidade nulo.");
        return;
    }

    //atualizando tempo
    float tempo_ate = subtraiData(horarioAtual, u->ultimoHorario);
    u->tempoAtendimento += tempo_ate;

    //atualizando data
    copiaData(&u->ultimoHorario, horarioAtual);

}


/*
 * \brief Atualiza o tempo ocioso de uma Unidade
 *
 * Essa função recebe uma Unidade e uma data, e atualiza
 * seu tempo ocioso, bem como o horarioAtual.
 * 
 * \param u A Unidade a ser atualizada.
 * \param horarioAtual A data do horário da atualização.
*/
void atualizaOciUnidade(Unidade *u, Data horarioAtual) {
    if (!u) {
        avisoAssert(u, "Unidade nulo.");
        return;
    }

    Data data_nula = inicializaData(0, 0, 0, 0);
    //atualizando tempo
    if (!dataIgual(u->ultimoHorario, data_nula)) {
        float tempo_oci = subtraiData(horarioAtual, u->ultimoHorario);
        u->tempoOcioso += tempo_oci;
    }

    //atualizando data
    copiaData(&u->ultimoHorario, horarioAtual);
}