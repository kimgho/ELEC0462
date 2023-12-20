#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

int main()
{
    int pid;
    int fd;
    printf("About to run who into a file\n");
    if ((fd = open("userlog", O_WRONLY | O_CREAT | O_APPEND, 0644)) == -1)
    {
        perror("open");
        exit(1);
    }

    if ((pid = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }

    if (pid == 0)
    {
        close(1);
        dup(fd);
        execlp("who", "who", NULL);
        perror("execlp");
        exit(1);
    }

    if (pid != 0)
    {
        wait(NULL);
        printf("Done running who. results in userlog\n");
    }
    close(fd);

    return 0;
}
