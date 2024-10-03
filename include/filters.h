#ifndef BENCHIANG_FILTERS_H
#define BENCHIANG_FILTERS_H

#include <stdlib.h>

void str_toupper(char *str, size_t str_len);
void str_tolower(char *str, size_t str_len);

int filter_apply(char filter, char *str, size_t str_len);

#endif
