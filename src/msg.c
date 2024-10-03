#include "msg.h"
#include <ctype.h>     // tolower
#include <stdio.h>     // fprintf
#include <stdlib.h>    // malloc
#include <string.h>    // strlen

int msg_init(Message *msg, char *input, char filter)
{
    msg->input  = input;
    msg->filter = filter;
    msg->size   = strlen(input) + 1;

    return 0;
}

int msg_decode(Message *msg, char *encoded_msg)
{
    if(encoded_msg == NULL)
    {
        return -1;
    }

    msg->input  = &encoded_msg[1];
    msg->filter = encoded_msg[0];
    msg->size   = strlen(encoded_msg) + 1;

    return 0;
}

int msg_encode(char *encoded_msg, const Message *msg)
{
    encoded_msg[0] = (char)tolower(msg->filter);
    strncpy(&encoded_msg[1], msg->input, msg->size);

    return 0;
}
