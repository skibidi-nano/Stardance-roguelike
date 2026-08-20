#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include "map.h"
#include "config.h"

void game_refresh(int ptr_player_y, int ptr_player_x);

int main(void)
{
    //setup for screen/ncurses
    initscr();
    cbreak();
    noecho();

    int player_y = MAP_HEIGHT / 2;
    int player_x = MAP_WIDTH / 2;

    int ch;

    
    game_refresh(player_y, player_x);

    while ((ch = getch()) != 'q')
    {

        switch(ch) 
        {
            case 'w': player_y--; break;
        }
        game_refresh(player_y, player_x);
    }

    endwin();
    return 0;
}

void game_refresh(int player_y, int player_x)
{
    clear();
    map_draw();
    mvaddch(player_y, player_x, '@');
    refresh();
}