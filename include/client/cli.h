// cppcheck-suppress-file unusedStructMember

typedef struct
{
    char *fifo_in;
    char *fifo_out;
    char *input;
    char  filter;
} Arguments;

void usage(const char *bin);
int  get_arguments(Arguments *args, int argc, char *argv[]);
