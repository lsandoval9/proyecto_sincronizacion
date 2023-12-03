#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>
#include <sys/time.h>

#include "../config/problema1_config.h"

extern struct timeval start_time;

#ifndef DATE_UTILITY_H

#define DATE_UTILITY_H

void print_time() 
{
    struct timeval current_time;

    gettimeofday(&current_time, NULL);

    if (start_time.tv_sec == 0 && start_time.tv_usec == 0) {
        start_time = current_time;
    }

    long elapsed_seconds = current_time.tv_sec - start_time.tv_sec;
    long elapsed_microseconds = current_time.tv_usec - start_time.tv_usec;

    if (elapsed_microseconds < 0) {
        elapsed_microseconds += 1000000;
        elapsed_seconds--;
    }

    elapsed_microseconds /= 1000; // convert to milliseconds

    //printf("<<<<< Time: %ld seconds %ld milliseconds >>>>>\n", elapsed_seconds, elapsed_microseconds);

    char time[256];
    snprintf(time, sizeof(time), "< Segundos: %ld | Milisegundos: %ld >", elapsed_seconds, elapsed_microseconds);

    //append_to_file(file_name, time);
    printf("%s\n", time);
}

#endif