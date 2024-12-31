#include "../include/Fila.h"
#include "../include/Paciente.h"
#include "../include/msgassert.h"

#include <stdlib.h>


#define QPROC 6
#define QFILA 3


/*
 * \brief Construtor de Fila
 *
 * Inicializa uma nova Fila, com três filas (verde, amarela
 * e vermelha), de tamanho máximo recebido como parâmetro,
 * que é o número de pacientes lidos.
 * 
 * \param tamMax Tamanho máximo da fila.
 */
Fila* inicializaFila(int qntdMax) {
    erroAssert(qntdMax > 0, "Tamanho invalido.");

    Fila* nova_fila = (Fila*) malloc(sizeof(Fila) * QFILA);
    erroAssert(nova_fila, "Memoria insuficiente.");

    for (int i = 0; i < QFILA; i++) {
        nova_fila[i].frente = 0;
        nova_fila[i].tras = 0;
        nova_fila[i].qntd = 0;
        nova_fila[i].tamMax = qntdMax;
        nova_fila[i].pacientes = (Paciente**) malloc(sizeof(Paciente*) * qntdMax);
        erroAssert(nova_fila[i].pacientes, "Memoria insuficiente");
        for (int j = 0; j < qntdMax; j++) nova_fila[i].pacientes[j] = NULL;
    }

    return nova_fila;
}


/*
 * \brief Insere um Paciente na Fila
 *
 * Recebe um Paciente e uma Fila e coloca esse paciente
 * na última posição da fila.
 * 
 * \param f Uma Fila.
 * \param p Um Paciente a ser inserido na Fila.
 */
void enfileira(Fila *f, Paciente *p) {
    if (!f || !p || f->qntd == f->tamMax) {
        avisoAssert(f, "Fila nula.");
        avisoAssert(p, "Paciente nulo.");
        avisoAssert(f->qntd != f->tamMax, "Fila cheia.");
        return;
    }

    f->pacientes[f->tras] = p;
    f->tras = (f->tras + 1) % f->tamMax;
    f->qntd++;
}


/*
 * \brief Remove o primeiro Paciente da Fila
 *
 * Remove o Paciente da primeira posição da Fila.
 * 
 * \param f Uma Fila.
 * \return O Paciente retirado da fila.
 */
Paciente* desinfileira(Fila *f) {
    if (!f || filaVazia(f)) {
        avisoAssert(f, "Fila nula.");
        avisoAssert(0, "Fila vazia.");
        return NULL;
    }

    Paciente *paciente_aux = f->pacientes[f->frente];
    f->pacientes[f->frente] = NULL;
    f->frente = (f->frente + 1) % f->tamMax;
    f->qntd--;

    return paciente_aux;
}


/*
 * \brief Verifica se uma Fila está vazia
 *
 * Essa função verifica se a Fila está vazia, retornando
 * 1 se estiver, ou 0 caso contrário.
 * 
 * \param f Uma Fila.
 * \return 0 ou 1 (não vazia ou vazia).
 */
int filaVazia(Fila *f) {
    if (!f) {
        avisoAssert(f, "Fila invalida.");
        return -1;
    }

    return (f->qntd ? 0 : 1);
}



void atualizaEstFilas(Fila **f, Data horario) {
    if (!f) {
        avisoAssert(f, "Fila invalida.");
        return;
    }

    for (int i = 0; i < QPROC; i++) {
        for (int j = 0; j < QFILA; j++) {
            for (int k = f[i][j].frente; k != f[i][j].tras; k = (k + 1) % f[i][j].tamMax) {
                atualizaOciPaciente(f[i][j].pacientes[k], horario);
            }    
        }
    }
        
}


/*
 * \brief Finaliza uma Fila
 *
 * Essa função recebe uma Fila e a finaliza, desalocando
 * as memórias ocupadas por essa estrutura.
 * 
 * \param f Uma Fila a ser finalizada.
 */
void finalizaFila(Fila **f) {
    if (!f) {
        avisoAssert(f, "Fila nula.");
        return;
    }

    for (int i = 0; i < QPROC; i++) {
        for (int j = 0; j < QFILA; j++) {
            free(f[i][j].pacientes);
        }
        free(f[i]);
    }

    free(f);
}