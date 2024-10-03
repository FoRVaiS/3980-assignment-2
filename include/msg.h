// cppcheck-suppress-file unusedStructMember

#ifndef BENCHIANG_MSG_H
#define BENCHIANG_MSG_H

#include <stdio.h>    // size_t

typedef struct
{
    char  *input;
    char   filter;
    size_t size;
} Message;

int msg_init(Message *msg, char *input, char filter);
int msg_decode(Message *msg, char *encoded_msg);
int msg_encode(char *encoded_msg, const Message *msg);

#endif
