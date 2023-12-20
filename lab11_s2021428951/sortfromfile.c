#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
int main(int ac, char *av[])
{
    int pid;
    int file_descriptor = open(av[1], O_RDONLY);
    if (file_descriptor == -1)
    {
        perror("open");
        exit(1);
    }
    printf("About to run sort with input from %s\n", av[1]);

    if ((pid = fork()) == -1)
    {
        perror("fork");
        close(file_descriptor);
        exit(1);
    }

    if (pid == 0)
    {
        if (dup2(file_descriptor, 0) == -1)
        {
            perror("dup2");
            close(file_descriptor);
            exit(1);
        }
        close(file_descriptor);
        execlp("sort", "sort", (char *)NULL);
        perror("execlp");
        return 1;
    }
    else
    {
        waitpid(pid, NULL, 0);
        printf("Done running sort < %s\n", av[1]);
    }

    return 0;
}
