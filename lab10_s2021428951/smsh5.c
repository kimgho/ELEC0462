#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include "smsh5.h"

#define DFL_PROMPT "> "

int VLenviron2table(char *env[]);

int main()
{
    char *cmdline, *prompt, **arglist;
    int result, process(char **, int);
    void setup();
    int status;
    prompt = DFL_PROMPT;
    setup();

    int background_job_number = 1;

    while ((cmdline = next_cmd(prompt, stdin)) != NULL &&
           strcmp(cmdline, "exit"))
    {
        if ((arglist = splitline(cmdline)) != NULL)
        {
            int i = 0;
            while (arglist[i] != NULL)
            {
                i++;
            }
            int background = 0;
            if (i > 1 && strcmp(arglist[i - 1], "&") == 0)
            {
                arglist[i - 1] = NULL;
                background = 1;
            }

            result = process(arglist, background);
        }
        free(cmdline);
    }

    return 0;
}

void setup()
{
    extern char **environ;

    VLenviron2table(environ);
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
{
    fprintf(stderr, "Error: %s, %s\n", s1, s2);
    exit(n);
}