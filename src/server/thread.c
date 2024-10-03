#include "server/thread.h"
#include <memory.h>
#include <pthread.h>
#include <stdio.h>

int thread_start(void *(*callback)(void *arg), ThreadArgs targs)
{
    pthread_t thread;

    int retthread;

    retthread = pthread_create(&thread, NULL, callback, &targs);
    if(retthread < 0)
    {
        return -1;
    }

    pthread_join(thread, NULL);

    return 0;
}
