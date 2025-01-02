#include "../include/Escalonador.h"
#include "../include/msgassert.h"
#include "../include/Paciente.h"

#include <stdlib.h>


/*
 * \brief Construtor de Escalonador
 *
 * Inicializa um novo Escalonador, com quantidade igual a 0
 * e inicializa as posições dos pacientes como nulo.
 */
Escalonador inicializaEscalonador(int qntdMax) {
    erroAssert(qntdMax > 0, "Tamanho invalido.");

    Escalonador novo_escalonador;
    novo_escalonador.qntd = 0;
    novo_escalonador.tamMax = qntdMax;
    novo_escalonador.pacientes = (Paciente**) malloc(sizeof(Paciente*) * qntdMax);
    erroAssert(novo_escalonador.pacientes, "Memoria insuficiente.");
    for(int i = 0; i < qntdMax; i++) novo_escalonador.pacientes[i] = NULL;

    return novo_escalonador;
}


/*
 * \brief Calcula a posição do ancestral de uma posicao
 *
 * Essa função calcula a posição do ancestral de uma posição
 * recebida de um Paciente e restorna essa posição.
 * 
 * \param posicao A posição do Paciente no vetor.
 * \return A posicão do ancestral do Paciente recebido.
 */
int getAncestral(int posicao) {
    if (posicao < 0) {
        erroAssert(posicao >= 0, "Posicao inválida.");
        return -1;
    }

    return (posicao - 1) / 2;
}


/*
 * \brief Calcula a posição do sucessor esquerdo de uma posição
 *
 * Essa função calcula a posição do sucessor esquerdo de uma
 * posição recebida de um Paciente e restorna essa posição.
 * 
 * \param posicao A posição do Paciente no vetor.
 * \return A posição do sucessor esquerdo do Paciente recebido.
 */
int getSucessorEsq(int posicao) {
    if (posicao < 0) {
        erroAssert(posicao >= 0, "Posicao inválida.");
        return -1;
    }

    return (2 * posicao) + 1;
}


/*
 * \brief Calcula a posição do sucessor direito de uma posição
 *
 * Essa função calcula a posição do sucessor direito de uma
 * posição recebida de um Paciente e restorna essa posição.
 * 
 * \param posicao A posição do Paciente no vetor.
 * \return A posição do sucessor direito do Paciente recebido.
 */
int getSucessorDir(int posicao) {
    if (posicao < 0) {
        erroAssert(posicao >= 0, "Posicao inválida.");
        return -1;
    }

    return (2 * posicao) + 2;
}


/*
 * \brief Insere um evento no Escalonador
 *
 * Essa função recebe um Paciente e um Escalonador
 * e insere o Paciente no Escalonador, seguindo as 
 * lógicas de um minHeap.
 * 
 * \param e Um Escalonador.
 * \param p Um Paciente a ser inserido.
*/
void insereEvento(Escalonador *e, Paciente *p) {
    if (!e || !p || e->qntd == e->tamMax) {
        avisoAssert(e, "Escalonador nulo.");
        avisoAssert(p, "Paciente nulo.");
        avisoAssert(e->qntd < e->tamMax, "Escalonador cheio");
        return;
    }

    e->pacientes[e->qntd] = p;

    //heapifyPorBaixo
    int i = e->qntd;
    int j = getAncestral(i);
    Paciente *paciente_aux;

    while (comparaPacientes(e->pacientes[i], e->pacientes[j], "<")) {
        paciente_aux = e->pacientes[i];
        e->pacientes[i] = e->pacientes[j];
        e->pacientes[j] = paciente_aux;

        i = j;
        j = getAncestral(i);
    }

    e->qntd++;
}


/*
 * \brief Remove um evento no Escalonador
 *
 * Essa função recebe um Escalonador e remove o Paciente
 * do Escalonador, seguindo as lógicas de um minHeap, ou seja,
 * remove a raiz (mínimo).
 * 
 * \param e Um Escalonador.
 * \return O paciente retirado do escalonador.
*/
Paciente* retiraProximoEvento(Escalonador *e) {
    if (!e || escalonadorVazio(e)) {
        avisoAssert(e, "Escalonador nulo.");
        avisoAssert(!escalonadorVazio(e), "Escalonador vazio.");
        return NULL;
    }

    Paciente *paciente_retirado = e->pacientes[0];
    e->pacientes[0] = e->pacientes[e->qntd - 1];
    e->pacientes[--e->qntd] = NULL;

    //heapifyPorCima
    Paciente *paciente_aux;
    int i = 0;
    int esq = getSucessorEsq(i), dir = getSucessorDir(i);

    if (esq < e->qntd) {
        if (esq < e->qntd && dir >= e->qntd) dir = esq;
        int s = (comparaPacientes(e->pacientes[esq], e->pacientes[dir], "<") ? esq : dir);

        while (comparaPacientes(e->pacientes[i], e->pacientes[s], ">")) {
            paciente_aux = e->pacientes[i];
            e->pacientes[i] = e->pacientes[s];
            e->pacientes[s] = paciente_aux;

            i = s;
            esq = getSucessorEsq(i), dir = getSucessorDir(i);
            if (esq >= e->qntd && dir >= e->qntd) break;
            if (esq < e->qntd && dir >= e->qntd) dir = esq;
            s = (comparaPacientes(e->pacientes[esq], e->pacientes[dir], "<") ? esq : dir);
        }
    }

    return paciente_retirado;
}


/*
 * \brief Verifica se o escalonador está vazio
 *
 * Essa função verifica se tem Pacientes no escalonador
 * e retorna 0 caso tenha, ou 1 caso não tenha.
 * 
 * \param e Um Escalonador a ser verificado se está vazio.
 * \return 0 ou 1 (não vazio ou vazio).
*/
int escalonadorVazio(Escalonador *e) {
    if (!e) {
        avisoAssert(e, "Escalonador nulo.");
        return -1;
    }
    
    return (e->qntd ? 0 : 1);
}


/*
 * \brief Atualiza as estatísticas dos Pacientes do Escalonador
 *
 * Essa função recebe um Escalonador e um horário, e atualiza 
 * o tempo de atendimento dos Pacientes do escalonador, caso
 * o paciente já esteja no Hospital.
 * 
 * \param e Um Escalonador a ser atualizado.
 * \param horario O horário da atualização.
*/
void atualizaEstEscalonador(Escalonador *e, Data horario) {
    for (int i = 0; i < e->qntd; i++) {
        if (e->pacientes[i] == NULL || e->pacientes[i]->estado == 1) continue;
            atualizaAtePaciente(e->pacientes[i], horario);
    }
}


/*
 * \brief Finaliza um Escalonador
 *
 * Essa função finaliza o Escalonador, liberando
 * a memória alocada.
 * 
 * \param e Um Escalonador a ser finalizado.
*/
void finalizaEscalonador(Escalonador *e) {
    if (!e) {
        avisoAssert(e, "Escalonador nulo.");
        return;
    }

    free(e->pacientes);
}