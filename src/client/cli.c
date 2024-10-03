#include "client/cli.h"
#include <stdio.h>     // fprintf
#include <unistd.h>    // getopt

void usage(const char *bin)
{
    printf("\nUsage: %s [-h] [-i <input_file>] [-o <output_file>] [-m <message>] [-f <filter>]\n", bin);
}

int get_arguments(Arguments *args, int argc, char *argv[])
{
    int retval;
    int opt;

    while((opt = getopt(argc, argv, "i:o:m:f:h")) != -1)
    {
        switch(opt)
        {
            case 'i':
                args->fifo_in = optarg;
                break;
            case 'o':
                args->fifo_out = optarg;
                break;
            case 'm':
                args->input = optarg;
                break;
            case 'f':
                args->filter = optarg[0];
                break;
            case 'h':
                retval = 0;
                goto exit;
            default:
                retval = -1;
                goto exit;
        }
    }

    if(args->fifo_in == NULL)
    {
        fprintf(stderr, "Input fifo path is required.\n");
        retval = -1;
        goto exit;
    }

    if(args->fifo_out == NULL)
    {
        fprintf(stderr, "Output fifo path is required.\n");
        retval = -1;
        goto exit;
    }

    if(args->input == NULL)
    {
        fprintf(stderr, "Message input is required.\n");
        retval = -1;
        goto exit;
    }

    if(args->filter == '\0')
    {
        fprintf(stderr, "Message filter is required.\n");
        retval = -1;
        goto exit;
    }

    retval = 0;

exit:
    return retval;
}
