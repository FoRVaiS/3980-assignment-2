// cppcheck-suppress-file unusedStructMember

#ifndef BENCHIANG_SERVER_THREAD_H
#define BENCHIANG_SERVER_THREAD_H

typedef struct
{
    int fd_input;
    int fd_output;
} ThreadArgs;

int thread_start(void *(*callback)(void *arg), ThreadArgs targs);

#endif
