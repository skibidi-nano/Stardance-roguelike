#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "menu.h"
#include "map.h"
#include "battle_screen.h"
#include "config.h"
#include "highscore.h"

void map_refresh(int player_y, int player_x);
void handle_menu_input(int user_input);
void handle_map_input(int user_input);
int handle_battle_input(int user_input);

//player starting position (prob will be changed)
    int player_y = 1;
    int player_x = 1;
    int battle_check = 0;
    int next_y = 0;
    int next_x = 0;
    int target_enemy_y, target_enemy_x;
    int score = 0;

gamestate current_gamestate = STATE_MENU;

int main(void)
{

    srand(time(NULL));

    //setup for screen/ncurses
    initscr();
    cbreak();
    noecho();

    //sets the map array up
    map_init();
    
    //first menu init
    menu_init();
    menu_draw();

    //main loop
    int user_input;
    while ((user_input = getch()) != 'q')
    {

        int selection = 0;
        switch (current_gamestate)
        {
            case STATE_MENU:
                handle_menu_input(user_input);
                break;

            case STATE_MAP:
                handle_map_input(user_input);
                break;

            case STATE_BATTLE:
                selection = handle_battle_input(user_input);
        }

        clear();

        switch (current_gamestate)
        {
            case STATE_MENU:
                menu_draw();
                break;

            case STATE_MAP:
                map_refresh(player_y, player_x);
                break;

            case STATE_BATTLE:
                battle_screen_draw(selection);
        }

        refresh();
    }

    endwin();
    return 0;
}

// refreshes the game (duh)
void map_refresh(int player_refresh_y, int player_refresh_x)
{
    clear();
    map_draw();
    mvaddch(player_refresh_y, player_refresh_x, '@');
    refresh();
}

void handle_menu_input(int user_input)
{
    menu_init();
    switch(user_input) 
    {
                case '1': 
                    map_init();       // generate a new room
                    reset_stats();    // reset player hp
                    player_y = 1;     // reset player position
                    player_x = 1;
                    current_gamestate = STATE_MAP; 
                    break;
    }
}

void handle_map_input(int user_input)
{
    next_y = player_y;
    next_x = player_x;

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
        target_enemy_y = next_y;
        target_enemy_x = next_x;
    }
    //wall collision check
    else if (!map_is_wall(next_y, next_x))
    {
        player_y = next_y;
        player_x = next_x;
    }    
}

int handle_battle_input(int user_input)
{
    static choice selection = ATTACK;
    int lock = 0;

    //to check for first battle frame
    if(battle_check == 0)
    {    
        battle_screen_init();
        battle_init();
        battle_check++;
    }

    //check for user input
    switch(user_input) 
    {
        case 'w': selection = ATTACK; break;
        case 's': selection = RUN; break;
        case ENTER : lock = ENTER; break;
    }
    
    static battle_result outcome = BATTLE_IN_PROGRESS;
    outcome = process_battle_turn(battle_check, selection, lock, target_enemy_x, target_enemy_y);
    lock = 0;
    if (outcome == BATTLE_VICTORY)
    {
        score = score_tracking(SCORE_FOR_DEFEATING_ENEMY);
        current_gamestate = STATE_MAP;
    }
    else if (outcome == BATTLE_DEFEAT)
    {
        score_register();
        current_gamestate = STATE_MENU;
    }

    return selection;
}