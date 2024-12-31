
#include "../include/Escalonador.h"
#include "../include/Hospital.h"
#include "../include/msgassert.h"

#include <string.h>


/*
 * \brief Função principal
 *
 * Essa função inicializa o Hospital HZ, simula os eventos
 * do hospital, imprime no terminal as informações dos
 * pacientes e finaliza o hospital. Recebe como parâmetro
 * o nome do arquivo de entrada.
*/
int main (int argc, char **argv) {
    char *nomeArq = argv[1];
    erroAssert(nomeArq, "Nome nao encontrado.");
    
    Hospital *hospitalHZ = preencheHospital(nomeArq);
    simulaHospital(hospitalHZ);
    imprimeHospital(hospitalHZ);
    finalizaHospital(hospitalHZ);

    return 0;
}