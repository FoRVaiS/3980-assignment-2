#ifndef BENCHIANG_NETWORK_H
#define BENCHIANG_NETWORK_H
#include <stdlib.h>

ssize_t send(const char *path, const char *msg, size_t msg_size);
ssize_t recv(const char *path, char *msg, size_t msg_size);

#endif
