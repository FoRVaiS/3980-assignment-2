#include "io.h"
#include <errno.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

ssize_t send(int fd, const char *buf, size_t buf_len, int *err)
{
    ssize_t retval;

    ssize_t nwritten;

    errno    = 0;
    nwritten = write(fd, buf, buf_len);
    if(nwritten < 0)
    {
        *err   = errno;
        retval = -1;
        goto exit;
    }

    retval = nwritten;

exit:
    return retval;
}

ssize_t recv(int fd, char **buf, size_t buf_len, int *err)
{
    ssize_t retval;

    ssize_t tread;
    ssize_t nread;

    if((*buf) != NULL)
    {
        retval = -1;
        goto exit;
    }

    nread = 0;
    *buf  = (char *)malloc(buf_len + 1);
    if(*buf == NULL)
    {
        retval = -2;
        goto exit;
    }
    memset(*buf, '\0', buf_len + 1);

    do
    {
        errno = 0;
        tread = read(fd, &(*buf)[nread], buf_len + 1);
        if(tread < 0)
        {
            *err   = errno;
            retval = -3;
            goto exit;
        }

        // Check if the message exceeds our expected buf len
        if(tread > (ssize_t)(buf_len))
        {
            retval = -4;
            goto exit;
        }

        nread += tread;
    } while(tread != 0);

    retval = nread;

exit:
    return retval;
}
