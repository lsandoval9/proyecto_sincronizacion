
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>


#ifndef JEFE_MESA_PROBLEMA2_H

#define JEFE_MESA_PROBLEMA2_H

// variables externas
extern sem_t mazo, reordenando, mutex_mazo;
extern int num_jugadores;
extern bool reordenado;
extern bool jefe_esperando;

void pensar_reordenamiento();
void reordenar_tablero();
void elegir_proxima_carta();
void colocar_carta_en_mazo(int carta);



void reordenar_tablero()
{
    jefe_esperando = true;

    if (reordenado) {
        sem_wait(&reordenando);
    }

    

}

void pensar_reordenamiento()
{
    printf("Jefe de mesa pensando reordenamiento\n");
    sleep(1);
}

void elegir_proxima_carta()
{
    printf("Jefe de mesa eligiendo proxima carta\n");
    sleep(1);
}

void colocar_carta_en_mazo(int carta)
{
    printf("Jefe de mesa colocando carta %d en mazo\n", carta);
    sleep(1);
}



#endif // JEFE_MESA_PROBLEMA2_H