#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include "map.h"
#include "config.h"

void game_refresh(int player_y, int player_x);

int main(void)
{
    //setup for screen/ncurses
    initscr();
    cbreak();
    noecho();

    //sets the map array up
    map_init();

    //player starting position (prob will be changed)
    int player_y = MAP_HEIGHT / 2;
    int player_x = MAP_WIDTH / 2;
    
    //inital map generation
    game_refresh(player_y, player_x);

    //further map generation for the rest of the game
    int ch;
    while ((ch = getch()) != 'q')
    {
            int next_y = player_y;
            int next_x = player_x;

        switch(ch) 
        {
            case 'w': next_y--; break;
            case 's': next_y++; break;
            case 'a': next_x--; break;
            case 'd': next_x++; break;
        }
        //wall collision check
        if (!map_is_wall(next_y, next_x))
        {
            player_y = next_y;
            player_x = next_x;
        }
        game_refresh(player_y, player_x);

    }

    endwin();
    return 0;
}

// refreshes the game (duh)
void game_refresh(int player_y, int player_x)
{
    clear();
    map_draw();
    mvaddch(player_y, player_x, '@');
    refresh();
}