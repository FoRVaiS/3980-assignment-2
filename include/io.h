#ifndef BENCHIANG_IO_H
#define BENCHIANG_IO_H

#include <stdlib.h>
#include <unistd.h>

ssize_t send(int fd, const char *buf, size_t buf_len, int *err);
ssize_t recv(int fd, char **buf, size_t buf_len, int *err);

#endif
