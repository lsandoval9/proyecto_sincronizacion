

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "time_utility.h" // Assuming the print_time function is defined in this header file
#include "file_utility.h"

#ifndef STOPWATCH_PROCESS_H

#define STOPWATCH_PROCESS_H

#define INTERVAL 1 // Interval in seconds


void* stopwatch_thread(void* arg) {
    int selected = *((int *)arg);
    char* file_name;
    switch(selected) {
        case 0:
            file_name = FILENAME_PROBLEMA1;
            break;
        case 1:
            file_name = FILENAME_PROBLEMA2;
            break;
    }
    while (1) {
        nanosleep((const struct timespec[]){{0, 500000000L}}, NULL); // Sleep for 0.1 seconds
        print_time(file_name); // Call the print_time function
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