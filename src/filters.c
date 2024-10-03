#include "filters.h"
#include <ctype.h>
#include <stdlib.h>

void str_toupper(char *str, size_t str_len)
{
    for(size_t idx = 0; idx < str_len; idx++)
    {
        str[idx] = (char)toupper(str[idx]);
    }
}

void str_tolower(char *str, size_t str_len)
{
    for(size_t idx = 0; idx < str_len; idx++)
    {
        str[idx] = (char)tolower(str[idx]);
    }
}
