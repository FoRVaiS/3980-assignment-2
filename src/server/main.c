#include "filters.h"
#include "io.h"
#include "msg.h"
#include "server/cli.h"
#include "server/thread.h"
#include <fcntl.h>
#include <memory.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

enum
{
    BUF_SIZE = 1024
};

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables,-warnings-as-errors])
static short shutdown = 0;

static void *handle_request(void *args);
static void  signal_handler(int signal_number);

int main(int argc, char *argv[])
{
    int retval;
    int retsa;

    int fd_input;
    int fd_output;

    struct sigaction sa;

    Arguments args;
    memset(&args, 0, sizeof(Arguments));

    retval = get_arguments(&args, argc, argv);
    if(retval < 0)
    {
        usage(argv[0]);
        goto exit;
    }

    // SIGINT Shutdown handling
#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#endif
    sa.sa_handler = signal_handler;
#if defined(__clang__)
    #pragma clang diagnostic pop
#endif
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    retsa = sigaction(SIGINT, &sa, NULL);
    if(retsa < 0)
    {
        fprintf(stderr, "Failed to set SIGINT handler.\n");
        retval = retsa;
        goto exit;
    }

    while(1)
    {
        ThreadArgs targs;

        printf("Waiting for client request...\n");

        // Open data pipes at the same time
        fd_input = open(args.fifo_in, O_RDONLY | O_CLOEXEC);

        if(shutdown)
        {
            if(fd_input > 0)
            {
                close(fd_input);
            }
            break;
        }

        if(fd_input < 0)
        {
            retval = -1;
            goto exit;
        }

        fd_output = open(args.fifo_out, O_WRONLY | O_CLOEXEC);
        if(fd_output < 0)
        {
            retval = -1;
            goto exit;
        }

        // Prepare thread context
        targs.fd_input  = fd_input;
        targs.fd_output = fd_output;

        // Spawn new thread to handle the request
        retval = thread_start(handle_request, targs);
        if(retval < 0)
        {
            fprintf(stderr, "Error spawning new thread.\n");
            goto exit;
        }

        // Close threads so client may read data
        close(fd_output);
        close(fd_input);
    }

    printf("Gracefully shutting down...\n");

exit:
    return retval;
}

void *handle_request(void *args)
{
    const ThreadArgs *targs;
    Message           msg;

    int err;

    int     fd_input;
    char   *buf;
    ssize_t nbytes_in;

    int     fd_output;
    ssize_t nbytes_out;

    // Define opened file descriptors
    targs     = (ThreadArgs *)args;
    fd_input  = targs->fd_input;
    fd_output = targs->fd_output;

    // Read entire input buffer
    buf       = NULL;
    err       = 0;
    nbytes_in = recv(fd_input, &buf, BUF_SIZE, &err);
    if(nbytes_in < 0)
    {
        const char *errmsg;

        if(nbytes_in == -1)
        {
            fprintf(stderr, "Buffer must be initialized to NULL.\n");
            goto cleanup;
        }
        else if(nbytes_in == -2)
        {
            fprintf(stderr, "Buffer failed to allocate memory.\n");
            goto cleanup;
        }
        else if(nbytes_in == -4)
        {    // Buf overflow
            fprintf(stderr, "Client sent a message larger than is accepted.\n");

            errmsg = "";

            free(buf);
            buf = (char *)malloc(strlen(errmsg) + 1);
            if(buf == NULL)
            {
                goto exit;
            }
            strncpy(buf, errmsg, strlen(errmsg) + 1);
        }
        else
        {
            errmsg = strerror(err);
            fprintf(stderr, "Failed to read bytes: %s\n", errmsg);
            goto cleanup;
        }
    }

    // Report received contents
    memset(&msg, '\0', sizeof(Message));
    msg_decode(&msg, buf);
    printf("\n");
    printf("Filter: \"%c\"\n", msg.filter);
    printf("Message: \"%s\"\n", msg.input);

    // Apply transformations
    filter_apply(msg.filter, msg.input, msg.size - 1);

    // Send transformed message to the client
    err        = 0;
    nbytes_out = send(fd_output, msg.input, msg.size - 1, &err);
    if(nbytes_out < 0)
    {
        const char *errmsg = strerror(err);
        fprintf(stderr, "Failed to write bytes: %s\n", errmsg);
        goto exit;
    }

    printf("--- End of thread\n");
    printf("\n");

cleanup:
    free(buf);
exit:
    return NULL;
}

static void signal_handler(int signal_number)
{
    if(signal_number == SIGINT)
    {
        shutdown = 1;
    }
}
