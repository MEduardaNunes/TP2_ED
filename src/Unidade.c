#include "../include/Unidade.h"


/*
 * \brief Construtor de Unidade
 *
 * Inicializa uma nova Unidade com os atributos
 * iguais a 0.
 * 
 * \return A nova Unidade.
 */
Unidade inicializaUnidade() {
    Unidade nova_unidade;

    nova_unidade.ocupado = 0;
    nova_unidade.tempoOcioso = 0.0;
    nova_unidade.ultimaData = 0;

    return nova_unidade;
}