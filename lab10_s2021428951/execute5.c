#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "varlib.h"

int excute(char **argv, int background)
{
    extern char **environ;
    pid_t pid;

    if (argv[0] == NULL)
        return 0;

    if ((pid = fork()) == -1)
        perror("fork fail");
    else if (pid == 0)
    {
        environ = VLtable2environ();
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        if (execvp(argv[0], argv))
        {
            perror("cannot excute");
            exit(1);
        }
    }
    else
    {
        if (!background)
        {
            waitpid(pid, NULL, 0);
        }
    }
    return pid;
}