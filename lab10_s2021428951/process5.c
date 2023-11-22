#include <stdio.h>

#include "smsh5.h"

int is_control_command(char *);
int do_control_command(char **);
int ok_to_excute();
int builtin_command(char **, int *);

int process(char **args, int background)
{
    int rv = 0;
    if (args[0] == NULL)
        rv = 0;
    else if (is_control_command(args[0]))
        rv = do_control_command(args);
    else if (ok_to_excute())
        if (!builtin_command(args, &rv))
            rv = excute(args, background);
    return rv;
}