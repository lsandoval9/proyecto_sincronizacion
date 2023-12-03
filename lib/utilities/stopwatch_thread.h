

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "time_utility.h" // Assuming the print_time function is defined in this header file

#ifndef STOPWATCH_PROCESS_H

#define STOPWATCH_PROCESS_H

#define INTERVAL 50000000L // Interval in seconds


void* stopwatch_thread(void* arg) {
    int selected = *((int *)arg);
    while (1) {
        nanosleep((const struct timespec[]){{0, INTERVAL}}, NULL); // Sleep for 0.1 seconds
        print_time(); // Call the print_time function
    }
}

void start_stopwatch(int input) {
     int *aux = malloc(sizeof(int)); // Allocate memory for the input value
    if (aux == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    *aux = input;
    printf("Input: %d\n", input);
    pthread_t thread;
    pthread_create(&thread, NULL, stopwatch_thread, aux); // Pass the address of aux as the argument
}


#endif