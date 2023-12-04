



#include <time.h>

#ifndef SLEEP_THREAD_UTILITY_H

#define SLEEP_THREAD_UTILITY_H

void recursos(long long nanoseconds) {

    if (nanoseconds <= 0) {
        return;
    }

    struct timespec duration;
    duration.tv_sec = nanoseconds / 1000000000;
    duration.tv_nsec = nanoseconds % 1000000000;
    nanosleep(&duration, NULL);
}


#endif //SLEEP_THREAD_UTILITY_H