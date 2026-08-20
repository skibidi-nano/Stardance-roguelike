#include <ncurses.h>
#include <stdlib.h>

const int height = 20;
const int width = 100;

int main(void)
{
    //setup for screen/ncurses
    initscr();
    cbreak();
    noecho(); 

    mvprintw(0, 0, "Press WASD to move or q to quit");

    char map[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int k = 0; k < width; k++)
        {
            if ((k == 0 || k == width -1) || (i == 0 || i == height -1))
            {
                map[i][k] = '#';
                mvprintw(i, k, "%c", map[i][k]);
            }
            else
            {
                map[i][k] = ' ';
            }
        }
    }

    int ch;
    while ((ch = getch()) != 'q')
    {
        clear();

        switch(ch) 
        {
            case 'w':
                mvprintw(2, 0, "@");
                break;
            case 'd':
                mvprintw(2, 0, "@");
                break;
            case 's':
                mvprintw(2, 0, "@");
                break;
            case 'a':
                mvprintw(2, 0, "@");
                break;

        }
        refresh();
    }

    endwin();
    return 0;
}