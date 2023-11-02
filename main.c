
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <pthread.h> 

#include "lib/example.h"

int main(int argc, char const *argv[])
{
    printMessage("Hello World!");
    return 0;
}
