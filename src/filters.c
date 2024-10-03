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

int filter_apply(char filter, char *str, size_t str_len)
{
    switch(tolower(filter))
    {
        case 'u':
            str_toupper(str, str_len);
            break;
        case 'l':
            str_tolower(str, str_len);
            break;
        case 'n':
            break;
        default:
            return -1;
    }

    return 0;
}
