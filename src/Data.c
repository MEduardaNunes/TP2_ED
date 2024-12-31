#include "../include/Data.h"
#include "../include/msgassert.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
 * \brief Construtor de Data
 *
 * Inicializa uma nova Data com as informações lidas de um
 * Paciente, sendo elas ano, mês, dia e hora.
 * 
 * \param a O ano da data.
 * \param m O mês da data.
 * \param d O dia da data.
 * \param h A hora da data.
 * \return A nova data.
 */
Data inicializaData(int a, int m, int d, float h) {
    erroAssert(a >= 0 && m >= 0 && d >= 0 && h >= 0, "Valores de data invalidos.");

    Data nova_data;

    nova_data.ano = a;
    nova_data.mes = m;
    nova_data.dia = d;
    nova_data.hora = h;

    return nova_data;
}


/*
 * \brief Soma uma data a uma quantidade de horas
 *
 * Essa função soma uma data recebida com uma quantidade
 * de horas e retorna a data final obtida.
 * 
 * \param d A data inicial.
 * \param horas A quantidade de horas a serem somadas.
 * \return A data resultante da soma.
 */
Data somaData(Data d, float horas) {
    Data nova_data;

    if (horas < 0) {
        avisoAssert(horas >= 0, "Horas inválida.");

        nova_data = inicializaData(0, 0, 0, 0);
        return nova_data;
    }

    int qntd_dias = 0;
    if (d.mes == 1 || d.mes == 3 || d.mes == 5 || d.mes == 7 ||
        d.mes == 8 || d.mes == 10 || d.mes == 12) qntd_dias = 31;
    else if (d.mes == 4 || d.mes == 6 || d.mes == 9 || d.mes == 11) qntd_dias = 30;
    else if (d.mes == 2 && (d.ano % 4 == 0)) qntd_dias = 29;
    else if (d.mes == 2) qntd_dias = 28;
    
    nova_data.ano = d.ano;
    nova_data.mes = d.mes;
    nova_data.dia = d.dia;
    nova_data.hora = d.hora + horas;

    if (nova_data.hora > 24.0) {
        int dia = (int) (nova_data.hora / 24.0);
        nova_data.hora -= dia * 24.0;
        nova_data.dia += dia;
    }

    if (nova_data.dia > qntd_dias) {
        int mes = (int) (nova_data.dia / qntd_dias);
        nova_data.dia -= mes * qntd_dias;
        nova_data.mes += mes;
    }

    if (nova_data.mes > 12) {
        int ano = (int) (nova_data.mes / 12);
        nova_data.mes -= ano * 12;
        nova_data.mes += ano;
    }

    return nova_data;
}


/*
 * \brief Subtrai duas datas
 *
 * Essa função subtrai duas datas e retorna a diferença
 * entre elas em horas.
 * 
 * \param dMaior A data maior.
 * \param dMenor A data menor.
 * \return A quantidade de horas de diferença entre as datas.
 */
float subtraiData(Data dMaior, Data dMenor) {
    float dif;

    if (dMaior.ano == dMenor.ano && dMaior.mes == dMenor.mes && dMaior.dia == dMenor.dia) {
        dif = dMaior.hora - dMenor.hora;

    } else if ((dMaior.ano == dMenor.ano && dMaior.mes == dMenor.mes && dMaior.dia != dMenor.dia) ||
        (dMaior.ano == dMenor.ano && dMaior.mes - 1 == dMenor.mes) ||
        (dMaior.ano - 1 == dMenor.ano && dMaior.mes == 1)) {
            int dia;
        if (dMaior.mes == dMenor.mes) dia = dMaior.dia - dMenor.dia - 1;
        else dia = dMenor.dia - 1;

        dif = (24.0 - dMenor.hora) + dMaior.hora + (float) dia * 24.0;
    }

    return dif;
}


/*
 * \brief Copia uma data
 *
 * Essa função copia em destino a data em origem.
 * 
 * \param destino A data que receberá a data de origem.
 * \param origem A data original.
 */
void copiaData(Data *destino, Data origem) {
    if (!destino) {
        avisoAssert(destino, "Data inválida.");
        return;
    }

    destino->ano = origem.ano;
    destino->mes = origem.mes;
    destino->dia = origem.dia;
    destino->hora = origem.hora;
}


/*
 * \brief Verifica se uma data é menor que outra
 *
 * Essa função verifica se a data em dMenor é realmente
 * menor que a data em dMaior. Retorna 1 caso positivo,
 * ou 0 caso contrário.
 * 
 * \param dMenor A possível data menor.
 * \param dMaior A possível data maior.
 * \return O resultado da comparação (0 ou 1).
 */
int dataMenor(Data dMenor, Data dMaior) {
    //ano
    if (dMenor.ano < dMaior.ano) return 1;
    else if (dMenor.ano > dMaior.ano) return 0;
    else {

        //mes
        if (dMenor.mes < dMaior.mes) return 1;
        else if (dMenor.mes > dMaior.mes) return 0;
        else {

            //dia
            if (dMenor.dia < dMaior.dia) return 1;
            else if (dMenor.dia > dMaior.dia) return 0;
            else {

                //hora
                if (dMenor.hora < dMaior.hora) return 1;
                else if (dMenor.hora > dMaior.hora) return 0;
                else return 0;
            }
        }
    }
}


/*
 * \brief Verifica se uma data é igua a outra
 *
 * Essa função verifica se a data d1 é igual a data d2.
 * Retorna 1 se isso for verdadeiro, ou 0 caso contrário.
 * 
 * \param d1 A primeira data.
 * \param d2 A segunda data.
 * \return O resultado da comparação (0 ou 1).
 */
int dataIgual(Data d1, Data d2) {
    return ((d1.ano == d2.ano) && (d1.mes == d2.mes) && (d1.dia == d2.dia) && (d1.hora == d2.hora));
}


/*
 * \brief Imprime uma data
 *
 * Essa função recebe uma data e exibe as informações
 * contidas nela no formato especificado.
 * 
 * \param d A data a ser impressa.
 */
void imprimeData(Data d) {
    char *dia = (char*) malloc(sizeof(char) * 4);
    erroAssert(dia, "Memoria insuficiente.");
    char *mes = (char*) malloc(sizeof(char) * 4);
    erroAssert(mes, "Memoria insuficiente.");

    //determinando hora, minuto e segundo da data
    int hora_int = (int) d.hora;

    float min = d.hora - (float) hora_int;
    min *= 60;
    int min_int = (int) min;

    float seg = min - (float) min_int;
    seg *= 60;

    if (seg >= 59.5 && seg <= 60.5) {
        seg = 0.0;
        min_int++;
    }

    //determinando dia da semana por meio do algoritmo de Zeller
    int dia_semana = (d.dia + (((d.mes + 1) * 13) / 5) + (d.ano % 100) + ((d.ano % 100) / 4) + (d.ano / 400) + (5 * (d.ano / 100))) % 7;
    if (dia_semana == 0) strcpy(dia, "Sun");
    else if (dia_semana == 1) strcpy(dia, "Sat");
    else if (dia_semana == 2) strcpy(dia, "Mon");
    else if (dia_semana == 3) strcpy(dia, "Tue");
    else if (dia_semana == 4) strcpy(dia, "Wed");
    else if (dia_semana == 5) strcpy(dia, "Thu");
    else if (dia_semana == 6) strcpy(dia, "Fri");
    else {
        avisoAssert(0, "Erro no algoritmo de Zeller");
        return;
    }

    //determinando mes
    if (d.mes == 1) strcpy(mes, "Jan");
    else if (d.mes == 2) strcpy(mes, "Feb");
    else if (d.mes == 3) strcpy(mes, "Mar");
    else if (d.mes == 4) strcpy(mes, "Apr");
    else if (d.mes == 5) strcpy(mes, "May");
    else if (d.mes == 6) strcpy(mes, "Jun");
    else if (d.mes == 7) strcpy(mes, "Jul");
    else if (d.mes == 8) strcpy(mes, "Aug");
    else if (d.mes == 9) strcpy(mes, "Sep");
    else if (d.mes == 10) strcpy(mes, "Oct");
    else if (d.mes == 11) strcpy(mes, "Nov");
    else if (d.mes == 12) strcpy(mes, "Dec");
    
    //impressao da data na formatacao do trabalho
    printf("%s %s %d ", dia, mes, d.dia);

    if (hora_int < 10) printf("0");
    printf("%d:", hora_int);

    if (min_int < 10) printf("0");
    printf("%d:", min_int);

    if (seg < 10) printf("0");
    printf("%.0f %d ", seg, d.ano);

    free(dia);
    free(mes);
}