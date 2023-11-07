#include <stdio.h>
#include <curses.h>
#include <unistd.h>
int main()
{
    initscr();
    int t = 0;
    while (1)
    {
        clear();
        move(10, 20);
        if (t & 1)
        {
            addstr("Hello, world");
        }
        move(LINES - 1, 0);
        refresh();
        t++;
        sleep(1);
    }
    endwin();
    return 0;
}