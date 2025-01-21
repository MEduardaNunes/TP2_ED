#include "../include/Escalonador.h"
#include "../include/Hospital.h"
#include "../include/msgassert.h"


#include <string.h>
#include <time.h>


void difTempo (struct timespec t1, struct timespec t2, struct timespec* res) {
    if (t2.tv_nsec < t1.tv_nsec) {
        res->tv_nsec = 1000000000 + t2.tv_nsec - t1.tv_nsec;
        res->tv_sec = t2.tv_sec - t1.tv_sec - 1;

    } else {
        res->tv_nsec = t2.tv_nsec - t1.tv_nsec;
        res->tv_sec = t2.tv_sec - t1.tv_sec;
    }
}


/*
 * \brief Função principal
 *
 * Essa função inicializa o Hospital HZ, simula os eventos
 * do hospital, imprime no terminal as informações dos
 * pacientes e finaliza o hospital. Recebe como parâmetro
 * o nome do arquivo de entrada.
*/
int main (int argc, char **argv) {
    struct timespec t1, t2, difTot;
    clock_gettime(CLOCK_MONOTONIC, &t1);

    char *nomeArq = argv[1];
    erroAssert(nomeArq, "Nome nao encontrado.");
    
    Hospital *hospitalHZ = preencheHospital(nomeArq);
    simulaHospital(hospitalHZ);
    imprimeHospital(hospitalHZ);
    finalizaHospital(hospitalHZ);

    clock_gettime(CLOCK_MONOTONIC, &t2);
    difTempo(t1, t2, &difTot);
    printf("Tempo total: %ld.%.9ld\n", difTot.tv_sec, difTot.tv_nsec);

    return 0;
}