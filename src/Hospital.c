#include "../include/Hospital.h"
#include "../include/Fila.h"
#include "../include/Escalonador.h"
#include "../include/msgassert.h"

#include <stdio.h>


/*
 * Variáveis globais para melhoria da legibilidade do código. Elas são:
 * As utilizadas para acessar os índices do atributo 'filas' do Hospital:
 * VERD - Verde (urgência 0)
 * AMAR - Amarelo (urgência 1)
 * VERM - Vermelho (urgência 2)
 * TRIA - Fila da triagem (índice 0)
 * ATEN - Fila de atendimento (índice 1)
 * MEDH - Fila de medidas hospitalares (índice 2)
 * TESL - Fila de testes de laboratório (índice 3)
 * EXAI - Fila de exames de imagem (índice 4)
 * INME - Fila de instrumentos/medicamentos (índice 5)
 * 
 * As utilizadas para padronizar o número de procedimentos e de filas:
 * QPROC - Quantidade de procedimentos (6)
 * QFILA - Quantidade de filas padrão (3, uma para cada urgência)
 */
#define VERD 0
#define AMAR 1
#define VERM 2

#define TRIA 0
#define ATEN 1
#define MEDH 2
#define TESL 3
#define EXAI 4
#define INME 5

#define QPROC 6
#define QFILA 3


/*
 * \brief Preenche um Hospital
 *
 * Essa função recebe um nome do arquivo e preenche um novo
 * Hospital com as informações lidas no arquivo. É responsável
 * por inicializar todos pacientes, procedimentos, filas e
 * escalonador do hospital.
 * 
 * \param nomeArq O nome do arquivo de entrada.
 * \return O novo Hospital preenchido.
*/
Hospital* preencheHospital(char *nomeArq) {
    //inicializar hospital
    Hospital *hospital = (Hospital*) malloc(sizeof(Hospital));
    erroAssert(hospital, "Memoria insuficiente");

    //abrindo arquivo
    FILE *arquivoEntrada = fopen(nomeArq, "r");
    erroAssert(arquivoEntrada, "Arquivo nao encontrado.");

    int qntd_atendente_proc;
    double tempo_proc;

    //leitura procedimentos
    hospital->procedimentos = (Procedimento**) malloc(sizeof(Procedimento*) * QPROC);
    erroAssert(hospital->procedimentos, "Memoria insuficiente");

    for (int i = 0; i < QPROC; i++) {
        fscanf(arquivoEntrada, "%lf %d", &tempo_proc, &qntd_atendente_proc);
        hospital->procedimentos[i] = inicializaProcedimento(tempo_proc, qntd_atendente_proc);
    }

    //leitura dos pacientes
    Paciente *paciente;
    int qntdPacientes, alta, ano, mes, dia, grauUrgencia, qMH, qTL, qEI, qIM;
    float id, hora;

    fscanf(arquivoEntrada, "%d", &qntdPacientes);
    hospital->qntdPacientes = qntdPacientes;
    hospital->pacientesHospital = (Paciente**) malloc(sizeof(Paciente*) * qntdPacientes);
    erroAssert(hospital->pacientesHospital, "Memoria insuficiente.");

    for (int i = 0; i < qntdPacientes; i++) {
        fscanf(arquivoEntrada, "%f %d %d %d %d %f %d %d %d %d %d", &id, &alta, &ano, &mes, &dia, &hora, &grauUrgencia, &qMH, &qTL, &qEI, &qIM);
        paciente = inicializaPaciente(id, alta, ano, mes, dia, hora, grauUrgencia, qMH, qTL, qEI, qIM);
        hospital->pacientesHospital[i] = paciente;

        for (int j = 0; j < QPROC; j++) {
            if (j == 0 || j == 1) paciente->tempoAtendido += hospital->procedimentos[j]->tempo;
            else if (!paciente->alta)
                paciente->tempoAtendido += (hospital->procedimentos[j]->tempo * paciente->quantidades[j - 2]);
        }
    }

    //inicializar filas
    hospital->filas = inicializaFilas(qntdPacientes);

    //inicializa escalonador
    hospital->escalonadorHospital = inicializaEscalonador(qntdPacientes);

    fclose(arquivoEntrada);
    return hospital;
}


/*
 * \brief Encontra o Procedimento que precisa ser atualizado de um Paciente
 *
 * Essa função recebe um Hospital e um paciente, e retorna o Procedimento 
 * que ele acabou de entrar ou de sair.
 * 
 * \param p O Paciente que será achado o Procedimento.
 * \param hospital O hospital em que se encontra o paciente.
 * \return O Procedimento encontrado.
*/
Procedimento* determinaProcedimento(Paciente *p, Hospital *hospital) {
    if (!p || !hospital) {
        avisoAssert(p, "Paciente nulo.");
        avisoAssert(hospital, "Hospital nulo.");
        return NULL;
    }

    if (p->estado == 1 || p->estado == 2) return NULL;
    else if (p->estado == 3 || p->estado == 4) return hospital->procedimentos[TRIA];
    else if (p->estado == 5 || p->estado == 6) return hospital->procedimentos[ATEN];
    else if (p->estado == 7 || p->estado == 8) return hospital->procedimentos[MEDH];
    else if (p->estado == 9 || p->estado == 10) return hospital->procedimentos[TESL];
    else if (p->estado == 11 || p->estado == 12) return hospital->procedimentos[EXAI];
    else if (p->estado == 13 || p->estado == 14) return hospital->procedimentos[INME];
}


/*
 * \brief Muda o estado de um Paciente
 *
 * Essa função recebe um Hospital e um paciente, e muda o
 * paciente para o próximo estado, além de atualizar a data final.
 * Também atualiza os dados da unidade a ser ocupada ou desocupada.
 * 
 * \param p O paciente a ser mudado de estado.
 * \param hospital O hospital em que se encontra o paciente.
*/
void mudaEstado(Paciente *p, Hospital *hospital) {
    if (!p || !hospital || p->estado == 14) {
        avisoAssert(p, "Paciente nulo.");
        avisoAssert(hospital, "Hospital nulo.");
        avisoAssert(p->estado < 14, "Paciente finalizado.");
        return;
    }

    //mudando estado e achando o procedimento
    p->estado++;
    if (p->estado == 2) return;

    Procedimento* proc = determinaProcedimento(p, hospital);
    double qntd_proc = determinaQuantidade(p);

    if (p->estado % 2 == 0) {
        //desocupando unidade
        if (p->idUnidade != -1) {
            proc->unidades[p->idUnidade].ocupado = 0;
            proc->unidades[p->idUnidade].ultimaData = hospital->relogioHospital;
            p->idUnidade = -1;
        }

        //atualizando estatisticas do paciente
        if (p->estado == 6 && p->alta) p->estado = 14; //paciente que teve alta
        p->dataFim = hospital->relogioHospital;

    } else {
        //atualizando tempo ocioso
        p->tempoOcioso += (double) ((hospital->relogioHospital - p->dataFim) / 3600.0);
        
        //ocupando unidade
        int id = achaUnidade(proc);
        if (id != -1) {
            p->idUnidade = id;
            proc->unidades[id].tempoOcioso += (double) ((hospital->relogioHospital - proc->unidades[id].ultimaData) / 3600.0);
            proc->unidades[id].ultimaData = hospital->relogioHospital;
            proc->unidades[id].ocupado = 1;
        }
        
        //atualizando tempo final
        double segs_totais = proc->tempo * qntd_proc * 3600;
        p->dataFim = hospital->relogioHospital + segs_totais;
    }   
}


/*
 * \brief Move um Paciente para uma Fila
 *
 * Essa função recebe um Hospital e um paciente, e o enfileira
 * na fila adequada, de acordo com seu estado.
 * 
 * \param p O Paciente que será enfileirado.
 * \param hospital O hospital em que se encontra o paciente.
*/
void moveParaFila(Paciente *p, Hospital *hospital) {
    if (!p || !hospital) {
        avisoAssert(p, "Paciente nulo.");
        avisoAssert(hospital, "Hospital nulo.");
        return;
    }

    if (p->estado == 2) enfileira(hospital->filas[TRIA], p);
    else if (p->estado == 4) enfileira(&hospital->filas[ATEN][p->grauUrgencia], p);
    else if (p->estado == 6) enfileira(&hospital->filas[MEDH][p->grauUrgencia], p);
    else if (p->estado == 8) enfileira(&hospital->filas[TESL][p->grauUrgencia], p);
    else if (p->estado == 10) enfileira(&hospital->filas[EXAI][p->grauUrgencia], p);
    else if (p->estado == 12) enfileira(&hospital->filas[INME][p->grauUrgencia], p);
}


/*
 * \brief Move um Paciente para um Atendimento
 *
 * Essa função recebe um Hospital, e verifica todos os procedimentos
 * por meio da função `verificaProcedimento`, mandando os Pacientes
 * que estão em alguma fila para o Procedimento desocupado.
 * 
 * \param hospital O hospital a ter seus atendimentos atualizados.
*/
void moveParaAtendimento(Hospital *hospital) {
    if (!hospital) {
        avisoAssert(hospital, "Hospital nulo.");
        return;
    }

    for (int i = 0; i < QPROC; i++)
        verificaProcedimento(i, hospital);
}


/*
 * \brief Verifica um Procedimento
 *
 * Essa função recebe um id e um Hospital, e verifica se tem pacientes
 * na fila de espera do Procedimento e se ele está desocupado. Caso 
 * isso seja verdade, esse paciente é movido para o Procedimento, seguindo
 * a ordem de prioridade.
 * 
 * \param id O id do Procedimento a ser verificado.
 * \param hospital O hospital em que se encontra o Procedimento.
*/
void verificaProcedimento(int id, Hospital *hospital) {
    if (id < 0  || id >= QPROC || !hospital) {
        avisoAssert(id >=0 && id < QPROC, "Id invalido.");
        avisoAssert(hospital, "Hospital nulo.");
        return;
    }

    Paciente *paciente;

    if (id == TRIA) {
        if (!filaVazia(hospital->filas[TRIA]) && !procedimentoOcupado(hospital->procedimentos[TRIA])) {
            paciente = desinfileira(hospital->filas[TRIA]);

            if (!paciente) {
                avisoAssert(paciente, "Paciente nulo.");
                return;
            }
            
            mudaEstado(paciente, hospital);
            insereEvento(&hospital->escalonadorHospital, paciente);
        }

    } else {
        if ((!filaVazia(&hospital->filas[id][VERD]) || !filaVazia(&hospital->filas[id][AMAR]) ||
        !filaVazia(&hospital->filas[id][VERM])) && !procedimentoOcupado(hospital->procedimentos[id])) {
            //removendo paciente de acordo com a prioridade
            if (!filaVazia(&hospital->filas[id][VERM])) paciente = desinfileira(&hospital->filas[id][VERM]);
            else if (!filaVazia(&hospital->filas[id][AMAR])) paciente = desinfileira(&hospital->filas[id][AMAR]);
            else if (!filaVazia(&hospital->filas[id][VERD])) paciente = desinfileira(&hospital->filas[id][VERD]);

            if (!paciente) {
                avisoAssert(paciente, "Paciente nulo.");
                return;
            }

            //inserindo evento no escalonador
            mudaEstado(paciente, hospital);
            insereEvento(&hospital->escalonadorHospital, paciente);
        }
    }
}


/*
 * \brief Simulador de eventos discretos
 *
 * Essa função recebe um Hospital e realiza todos os eventos
 * desse hospital, sendo eles: enviar os pacientes para 
 * procedimentos e filas necessárias. Além disso, atualiza os
 * tempos ociosos de cada Paciente.
 * 
 * \param hospital O Hospital a ser simulado.
*/
void simulaHospital(Hospital *hospital) {
    if (!hospital) {
        avisoAssert(hospital, "Hospital nulo.");
        return;
    }

    //inserir todos pacientes no escalonador
    for (int i = 0; i < hospital->qntdPacientes; i++)
        insereEvento(&hospital->escalonadorHospital, hospital->pacientesHospital[i]);

    //inicializando horas das unidades
    for (int i = 0; i < QPROC; i++)
        inicializaHora(hospital->procedimentos[i], hospital->escalonadorHospital.pacientes[0]->dataFim);

    //simular eventos no hospital
    while (!escalonadorVazio(&hospital->escalonadorHospital)) {
        //proximo evento
        Paciente *paciente = retiraProximoEvento(&hospital->escalonadorHospital);
        if (!paciente) {
            avisoAssert(paciente, "Paciente nulo.");
            continue;
        }

        //atualizando horario do relogio do hospital
        hospital->relogioHospital = paciente->dataFim;

        //mudando estado do paciente, enfileirando
        //movendo para atendimento e atualizando estatísticas
        mudaEstado(paciente, hospital);
        if (paciente->estado != 14) moveParaFila(paciente, hospital);
        moveParaAtendimento(hospital);
    }

    //finalizando hroas das unidades
    for (int i = 0; i < QPROC; i++)
        finalizaHora(hospital->procedimentos[i], hospital->relogioHospital);
}


/*
 * \brief Imprime um Hospital
 *
 * Essa função recebe um Hospital e imprime os Pacientes
 * desse Hospital na formatação específica.
 * 
 * \param hospital O hospital a ser impresso.
*/
void imprimeHospital(Hospital *hospital) {
    if (!hospital) {
        avisoAssert(hospital, "Hospital nulo.");
        return;
    }

    for (int i = 0; i < hospital->qntdPacientes; i++)
        imprimePaciente(hospital->pacientesHospital[i]);
}


/*
 * \brief Finaliza um Hospital
 *
 * Essa função recebe um Hospital e desaloca todas as
 * memórias alocadas para essa estrutura, chamando
 * outros finalizadores de estruturas.
 * 
 * \param hospital O hospital a ser finalizado.
*/
void finalizaHospital(Hospital *hospital) {
    if (!hospital) {
        avisoAssert(hospital, "Hospital nulo.");
        return;
    }

    //desalocar memorias
    for (int i = 0; i < hospital->qntdPacientes; i++)
        free(hospital->pacientesHospital[i]);
    free(hospital->pacientesHospital);
    finalizaFilas(hospital->filas);
    finalizaProcedimentos(hospital->procedimentos);
    finalizaEscalonador(&hospital->escalonadorHospital);
    free(hospital);
}