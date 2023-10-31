#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

void do_more(FILE *);
int see_more();

int main(int ac, char *av[])
{
    FILE *fp;
    if (ac == 1)
    {
        do_more(stdin);
    }
    else
    {
        while (--ac)
        {
            if ((fp = fopen(*++av, "r")) != NULL)
            {
                do_more(fp);
                fclose(fp);
            }
            else
            {
                exit(1);
            }
        }
    }
    return 0;
}
void do_more(FILE *fp)
{

    struct winsize wbuf;
    int oLineLen = 0;
    int oPageLen = 0;
    if (ioctl(0, TIOCGWINSZ, &wbuf) != -1)
    {
        oLineLen = wbuf.ws_col;
        oPageLen = wbuf.ws_row;
    }
    char line[oLineLen];
    int num_of_lines = 0;
    int see_more(), reply;
    while (fgets(line, oLineLen, fp))
    {
        if (num_of_lines == oPageLen)
        {
            reply = see_more();
            if (reply == 0)
            {
                break;
            }
            num_of_lines -= reply;
        }
        if (fputs(line, stdout) == EOF)
        {
            exit(1);
        }
        num_of_lines++;
    }
}
int see_more()
{
    struct winsize wbuf;
    int oLineLen = 0;
    int oPageLen = 0;
    int c;
    printf("\033[7m more? \033[m");
    while ((c = getchar()) != EOF)
    {
        if (c == 'q')
            return 0;
        if (c == ' ')
        {
            if (ioctl(0, TIOCGWINSZ, &wbuf) != -1)
            {
                return oPageLen = wbuf.ws_row;
            }
        }
        if (c == '\n')
            return 1;
    }
    return 0;
}
