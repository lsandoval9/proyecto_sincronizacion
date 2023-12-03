/**
 * @file main.c
 * @brief Archivo principal del programa - Proyecto: sincronizacion de entre procesos
 * @author Luis A. Sandoval
 * @author Gustavo Herrera
 * @author Jose David Lopez
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "lib/problema2/problema2.h"
#include "lib/problema1/problema1.h"
#include "lib/utilities/file_utility.h"
#include "lib/utilities/time_utility.h"
#include "lib/utilities/stopwatch_thread.h"

struct timeval start_time;

int main(int argc, char const *argv[])
{
    /**
     * @brief argumento para seleccionar opcion
     */
    int input;

    /**
     * @brief Obtener el tiempo de inicio del programa
    */
    gettimeofday(&start_time, NULL);

    

    /**
     * @brief si no se ingresa argumento, se toma el valor 0 por defecto
     */
    if (argc < 2)
    {
        input = 0;
    }
    else
    {
        input = atoi(argv[1]);
    }

    start_stopwatch(input);

    switch (input)
    {
    case 0:
        iniciarProblema1();
        break;
    case 1:
        iniciarProblema2();
        break;
    default:
        printf("Uso del programa: ./exec.bash [opcion:int1]\n");
        printf("Opciones:\n");
        printf("0: Problema 1\n");
        printf("1: Problema 2\n");
        break;
    }

    return 0;
}
