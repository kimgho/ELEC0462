#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

time_t current_time = 0;
int elapsed_time = 0;

int main()
{
    void f();
    signal(SIGINT, f);

    time_t start_time = time(NULL);

    printf("you can't stop me!\n");
    while (1)
    {
        sleep(1);
        current_time = time(NULL);
        elapsed_time = current_time - start_time;
        printf("haha\n");
    }

    return 0;
}

void f() { printf("Currently elapsed time:  %d sec(s)\n", elapsed_time); }