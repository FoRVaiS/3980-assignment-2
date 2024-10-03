// cppcheck-suppress-file unusedStructMember

#ifndef BENCHIANG_SERVER_CLI_H
#define BENCHIANG_SERVER_CLI_H

typedef struct
{
    char *fifo_in;
    char *fifo_out;
} Arguments;

void usage(const char *bin);
int  get_arguments(Arguments *args, int argc, char *argv[]);

#endif
