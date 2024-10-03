#include "client/cli.h"
#include "io.h"
#include "msg.h"
#include <errno.h>     // errno
#include <fcntl.h>     // open, O_RDONLY, OCLOEXEC
#include <stdio.h>     // fprintf, stderr
#include <string.h>    // strerror, strlen
#include <unistd.h>    // close

int main(int argc, char *argv[])
{
    int retval;
    int err;

    Message msg;

    int     fd_out;
    char   *msg_out;
    ssize_t nbytes_out;

    int     fd_in;
    char   *msg_in;
    ssize_t nbytes_in;

    Arguments args;
    memset(&args, 0, sizeof(Arguments));

    retval = get_arguments(&args, argc, argv);
    if(retval < 0)
    {
        usage(argv[0]);
        goto exit;
    }

    // Prepare message
    if(strlen(args.input) == 0)
    {
        // Empty inputs should not be allowed
        retval = -1;
        fprintf(stderr, "Empty inputs are not permitted, inputs should contain at least one character\n");
        goto exit;
    }

    msg_init(&msg, args.input, args.filter);
    msg_out = (char *)malloc(msg.size);    // +1 for null term
    if(msg_out == NULL)
    {
        fprintf(stderr, "Failed to allocate %zu bytes for msg_out\n", msg.size + 1);
        goto free_msg_out;
    }

    msg_encode(msg_out, &msg);
    if(msg_out[0] != 'u' && msg_out[0] != 'l' && msg_out[0] != 'n')
    {
        fprintf(stderr, "Invalid filter '%c'\n", msg_out[0]);
        goto free_msg_out;
    }

    // Send message to server
    errno  = 0;
    fd_out = open(args.fifo_out, O_WRONLY | O_CLOEXEC);
    if(fd_out < 0)
    {
        const char *errmsg = strerror(errno);
        fprintf(stderr, "Failed to open output file: %s\n", errmsg);
        retval = errno;
        goto free_msg_out;
    }

    err        = 0;
    nbytes_out = send(fd_out, msg_out, msg.size, &err);
    if(nbytes_out < 0)
    {
        const char *errmsg = strerror(err);
        fprintf(stderr, "Failed to write to file: %s\n", errmsg);
        retval = errno;
        goto free_msg_out;
    }

    close(fd_out);    // fifo has to be closed in order for it to be read

    // Wait for server response
    errno = 0;
    fd_in = open(args.fifo_in, O_RDONLY | O_CLOEXEC);
    if(fd_in < 0)
    {
        const char *errmsg = strerror(errno);
        fprintf(stderr, "Failed to open input fifo: %s\n", errmsg);
        retval = errno;
        goto free_msg_out;
    }

    err       = 0;
    msg_in    = NULL;
    nbytes_in = recv(fd_in, &msg_in, strlen(args.input) + 1, &err);
    if(nbytes_in < 0)
    {
        const char *errmsg;

        if(nbytes_in == -1)
        {
            fprintf(stderr, "Buffer must be initialized to NULL.\n");
        }
        else if(nbytes_in == -2)
        {
            fprintf(stderr, "Buffer failed to allocate memory.\n");
        }
        else if(nbytes_in == -4)
        {
            fprintf(stderr, "Server sent a message larger than is accepted.\n");
            goto free_msg_in;
        }
        else
        {
            errmsg = strerror(err);
            fprintf(stderr, "Failed to read bytes: %s\n", errmsg);
            goto free_msg_in;
        }
    }

    if(strcmp(msg_in, "") == 0)
    {
        // The only error the server emits is the buf_overflow error.
        fprintf(stderr, "Server could not process message of length %zu.\n", strlen(msg.input));
        goto free_msg_in;
    }

    printf("Message: %s\n", msg_in);

    close(fd_in);

    retval = 0;

free_msg_in:
    free(msg_in);
free_msg_out:
    free(msg_out);
exit:
    return retval;
}
