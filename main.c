#include <ncurses.h>
#include <stdlib.h>
#include "menu.h"
#include "map.h"
#include "battle_screen.h"
#include "config.h"

void map_refresh(int player_y, int player_x);

int main(void)
{
    //setup for screen/ncurses
    initscr();
    cbreak();
    noecho();

    //sets the map array up
    map_init();

    gamestate current_gamestate = STATE_MENU;

    //player starting position (prob will be changed)
    int player_y = 1;
    int player_x = 1;
    int battle_check = 0;
    
    //first menu init
    menu_init();
    menu_draw();

    //main loop
    int user_input;
    while ((user_input = getch()) != 'q')
    {

        if (current_gamestate == STATE_MENU)
        {
            menu_init();
            menu_draw();
            switch(user_input) 
            {
                case '1': current_gamestate = STATE_MAP; break;
            }
        }

        if (current_gamestate == STATE_MAP)
        {
            int next_y = player_y;
            int next_x = player_x;

            //action based on input
            switch(user_input) 
            {
                case 'w': next_y--; break;
                case 's': next_y++; break;
                case 'a': next_x--; break;
                case 'd': next_x++; break;
            }

            //door collision check
            if (map_is_door(next_y, next_x))
            {
                map_init();
                player_y = 1;
                player_x = 1;
            }
            //player-enemy collision check
            else if(map_is_enemy(next_y, next_x))
            {
                current_gamestate = STATE_BATTLE;
                battle_check = 0;
            }
            //wall collision check
            else if (!map_is_wall(next_y, next_x))
            {
                player_y = next_y;
                player_x = next_x;
            }

            map_refresh(player_y, player_x);
        }

        if (current_gamestate == STATE_BATTLE)
        {
            static int selection = 1;
            static int lock = 0;
            if(battle_check == 0)
            {    
                battle_screen_init();
                battle_init();
                battle_check++;
            }

            switch(user_input) 
            {
                case 'w': selection--; break;
                case 's': selection++; break;
                case ENTER : lock = ENTER; break;
            }
            battle_screen_draw();
            static int outcome = 0;
            outcome = process_battle_turn(battle_check, selection, lock);
            if (outcome == 1)
            {
                current_gamestate = STATE_MAP;
            }
            else if (outcome == 0)
            {
                mvaddch(10, 10, 'g');
                //need to add highscore/score function
                //need to add sleep function
                current_gamestate = STATE_MENU;
            }
        }
        
    }

    endwin();
    return 0;
}

// refreshes the game (duh)
void map_refresh(int player_y, int player_x)
{
    clear();
    map_draw();
    mvaddch(player_y, player_x, '@');
    refresh();
}