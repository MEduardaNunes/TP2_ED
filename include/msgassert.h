#ifndef MSGASSERT_H
#define MSGASSERT_H

#include <stdio.h>
#include <stdlib.h>


/*
 * \brief Macro de aviso
 *
 * Macro que recebe como parâmetro uma expressão a ser verificada
 * e uma mensagem. Caso a expressão seja inválida, uma mensagem de
 * aviso é impressa no 'stderr', contendo o arquivo, a linha, a 
 * expressão inválida e a mensagem associada ao aviso.
 * 
 * \param esperado Expressao logica que espera ser verdadeira.
 * \param msg Mensagem associada ao erro.
*/
#define avisoAssert(esperado, msg) ((void) ((esperado) ? 0 : __avisoAssert(#esperado, __FILE__, __LINE__, msg)))
#define __avisoAssert(esperado, arq, linha, msg) ((void) fprintf(stderr, "%s:linha %u: Aviso: '%s' - %s\n", arq, linha, esperado, msg), 0)


/*
 * \brief Macro de erro
 *
 * Macro que recebe como parâmetro uma expressão a ser verificada
 * e uma mensagem. Caso a expressão seja inválida, uma mensagem de
 * aviso é impressa no 'stderr', contendo o arquivo, a linha, a 
 * expressão inválida e a mensagem associada ao erro. O programa
 * é abortado.
 * 
 * \param esperado Expressao logica que espera ser verdadeira.
 * \param msg Mensagem associada ao erro.
*/
#define erroAssert(esperado, msg) ((void) ((esperado) ? 0 : __erroAssert(#esperado, __FILE__, __LINE__, msg)))
#define __erroAssert(esperado, arq, linha, msg) ((void) fprintf(stderr, "%s:linha %u: Erro: '%s' - %s\n", arq, linha, esperado, msg), abort(), 0)

#endif