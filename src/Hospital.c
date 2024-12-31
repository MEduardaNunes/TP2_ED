#include "../include/Hospital.h"
#include "../include/Fila.h"
#include "../include/Escalonador.h"
#include "../include/msgassert.h"

#include <stdio.h>


/*
 * Variáveis globais para melhoria da legibilidade do código. Elas são:
 * VERD - Verde (urgência 0)
 * AMAR - Amarelo (urgência 1)
 * VERM - Vermelho (urgência 2)
 * Utilizadas para acessar os índices do atributo 'filas' do Hospital.
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


/*    t
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
    float tempo_proc;

    //leitura procedimentos
    hospital->procedimentos = (Procedimento**) malloc(sizeof(Procedimento*) * QPROC);
    erroAssert(hospital->procedimentos, "Memoria insuficiente");

    for (int i = 0; i < QPROC; i++) {
        fscanf(arquivoEntrada, "%f %d", &tempo_proc, &qntd_atendente_proc);
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
    }

    //inicializar filas
    hospital->filas = (Fila **) malloc(sizeof(Fila*) * QPROC);
    erroAssert(hospital->filas, "Memoria insuficiente");
    for (int i = 0; i < QPROC; i++) {
        hospital->filas[i] = inicializaFila(qntdPacientes);
    }
    
    //inicializa escalonador
    hospital->escalonadorHospital = inicializaEscalonador(qntdPacientes);

    //inicializa data
    hospital->relogioHospital = inicializaData(0, 0, 0, 0);

    fclose(arquivoEntrada);
    return hospital;
}


/*
 * \brief Encontra o Procedimento que precisa ser atualizado de um Paciente
 *
 * Essa função recebe um Hospital e um paciente, e retorna o Procedimento 
 * que ele acabou de entrar ou de sair.
 * 
 * \param paciente O Paciente que será achado o Procedimento.
 * \param hospital O hospital em que se encontra o paciente.
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
 * \brief Calcula tempo total de atendimento
 *
 * Essa função recebe um Hospital e um paciente, e calcula o 
 * tempo que o Paciente ficará no procedimento que ele está.
 * 
 * \param paciente O Paciente que será calculado o tempo de atendimento.
 * \param hospital O hospital em que se encontra o paciente.
*/
float tempoTotalAtendimento(Paciente *p, Hospital *hospital) {
    if (!p || !hospital) {
        avisoAssert(p, "Paciente nulo.");
        avisoAssert(hospital, "Hospital nulo.");
        return 0.0;
    }

    Procedimento *proc = determinaProcedimento(p, hospital);
    if (!proc) {
        avisoAssert(proc, "Procedimento nulo.");
        return 0.0;
    }

    float qntd = 0.0, tempo_total;

    if (p->estado == 3 || p->estado == 5) qntd = 1.0;
    else {
        int i = (p->estado - 7) / 2;
        qntd = (float) p->quantidades[i];
    }
    tempo_total = qntd * proc->tempo;

    return tempo_total;
}


/*
 * \brief Muda o estado de um Paciente
 *
 * Essa função recebe um Hospital e um paciente, e muda o
 * paciente para o próximo estado, além de atualizar as 
 * datas do atributo 'periodoAtual'. Também atualiza os 
 * dados da unidade a ser ocupada ou desocupada.
 * 
 * \param paciente O paciente a ser mudado de estado.
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
    if(p->estado == 2) return;

    Procedimento* proc = determinaProcedimento(p, hospital);
    if (p->estado % 2 == 0) {
        //desocupando unidade
        if (p->idUnidade != -1) {
            atualizaAteUnidade(&proc->unidades[p->idUnidade], hospital->relogioHospital);
            proc->unidades[p->idUnidade].ocupado = 0;
            printf("DESOC %d ", p->idUnidade);
            p->idUnidade = -1;
        }

        //atualizando estatisticas do paciente
        atualizaAtePaciente(p, hospital->relogioHospital);
        if (p->estado == 6 && p->alta) p->estado = 14;
        copiaData(&p->dataFim, hospital->relogioHospital);

    } else {
        //ocupando unidade vazia
        int unidade = achaUnidadeVazia(proc);
        if (unidade == -1) {
            avisoAssert(unidade != -1, "Procedimento ocupado.");
            return;
        }

        p->idUnidade = unidade;
        ocupaUnidade(proc, unidade, hospital->relogioHospital);
        
        //atualizando estatisticas do paciente
        printf("OCUPA %d ", unidade);
        atualizaOciPaciente(p, hospital->relogioHospital);
        Data data_final = somaData(hospital->relogioHospital, tempoTotalAtendimento(p, hospital));
        copiaData(&p->dataInicio, hospital->relogioHospital);
        copiaData(&p->dataFim, data_final);
    }   
}


/*
 * \brief Move um Paciente para uma Fila
 *
 * Essa função recebe um Hospital e um paciente, e o enfileira
 * na fila adequada, de acordo com seu estado.
 * 
 * \param paciente O Paciente que será enfileirado.
 * \param hospital O hospital em que se encontra o paciente.
*/
void moveParaFila(Paciente *p, Hospital *hospital) {
    if (!p || !hospital) {
        avisoAssert(p, "Paciente nulo.");
        avisoAssert(hospital, "Hospital nulo.");
        return;
    }

    if (p->estado == 2) enfileira(&hospital->filas[TRIA][p->grauUrgencia], p);
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
 * isso seja verdade, esse paciente é movido para o Procedimento.
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


/*
 * \brief Simulador de eventos discretos
 *
 * Essa função recebe um Hospital e realiza todos os eventos
 * desse hospital, sendo eles: enviar os pacientes para 
 * procedimentos e filas necessárias. Além disso, atualiza os
 * tempos ociosos e de atendimento de cada Paciente e de Unidade.
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

    //simular eventos no hospital
    while (!escalonadorVazio(&hospital->escalonadorHospital)) {
        //proximo evento
        Paciente *paciente = retiraProximoEvento(&hospital->escalonadorHospital);
        if (!paciente) {
            avisoAssert(paciente, "Paciente nulo.");
            continue;
        }

        //atualizando horario do relogio do hospital
        copiaData(&hospital->relogioHospital, paciente->dataFim);

        //mudando estado do paciente, enfileirando
        //movendo para atendimento e atualizando estatísticas
        mudaEstado(paciente, hospital);
        if (paciente->estado != 14) moveParaFila(paciente, hospital);
        moveParaAtendimento(hospital);
        atualizaEstHospital(hospital);
        printf("\n");
    }
}


/*
 * \brief Atualiza as estatísticas do Hospital
 *
 * Essa função recebe um Hospital e atualiza os tempos ociosos
 * e de atendimento de todos Pacientes do hospital e de todas
 * Unidades dos Procedimentos do hospital.
 * 
 * \param hospital O Hospital a ser atualizado.
*/
void atualizaEstHospital(Hospital *hospital) {
    if (!hospital) {
        avisoAssert(hospital, "Hospital nulo.");
        return;
    }

    //atualizar pacientes
    atualizaEstEscalonador(&hospital->escalonadorHospital, hospital->relogioHospital);
    atualizaEstFilas(hospital->filas, hospital->relogioHospital);

    //atualizar unidades
    atualizaEstProcedimentos(hospital->procedimentos, hospital->relogioHospital);
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
    finalizaFila(hospital->filas);
    finalizaProcedimentos(hospital->procedimentos);
    finalizaEscalonador(&hospital->escalonadorHospital);
    free(hospital);
}