#include "network.h"
#include <errno.h>     // errno
#include <fcntl.h>     // open
#include <stdio.h>     // fprintf
#include <unistd.h>    // write, close

ssize_t send(const char *path, const char *msg, size_t msg_size)
{
    int     fd            = -1;
    ssize_t bytes_written = 0;

    errno = 0;

    fd = open(path, O_WRONLY | O_CLOEXEC);
    if(fd == -1)
    {
        return -1;
    }

    bytes_written = write(fd, msg, msg_size);

    close(fd);

    if(bytes_written <= 0)
    {
        fprintf(stderr, "Could not write to %s\n", path);
        return -1;
    }

    return 0;
}

ssize_t recv(const char *path, char *msg, size_t msg_size)
{
    int     fd         = -1;
    ssize_t bytes_read = 0;

    errno = 0;

    fd = open(path, O_RDONLY | O_CLOEXEC);
    if(fd == -1)
    {
        fprintf(stderr, "Failed to open %s", path);
        return -1;
    }

    bytes_read = read(fd, msg, msg_size);

    close(fd);

    if(bytes_read == -1)
    {
        fprintf(stderr, "Failure during byte reading.\n");
        return -1;
    }

    return bytes_read;
}
