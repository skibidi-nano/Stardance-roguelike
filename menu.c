#include <ncurses.h>
#include "menu.h"
#include "config.h"
#include "highscore.h"

static char menu[MENU_HEIGHT][MENU_WIDTH];


//fill the menu array
void menu_init(void)
{
    for (int y = 0; y < MENU_HEIGHT; y++)
    {
        for (int x = 0; x < MENU_WIDTH; x++)
        {
            if (y == 0 || y == MENU_HEIGHT -1 || x == 0 || x == MENU_WIDTH -1)
            {
                menu[y][x] = '#';
            }
            else
            {
                menu[y][x] = ' '; 
            }
        }
    }
}

//draw the menu
void menu_draw(void)
{
    for (int y = 0; y < MENU_HEIGHT; y++)
    {
        for (int x = 0; x < MENU_WIDTH; x++)
        {
            mvaddch(y, x, menu[y][x]);
        }
    }

    int highscore = get_highscore();
    mvprintw(2, 13, "CURRENT HIGHSCORE: %i", highscore);
    mvprintw(4, 19, "[1] START GAME");
    mvprintw(6, 19, "[q] QUIT GAME");

}