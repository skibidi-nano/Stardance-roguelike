#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "menu.h"
#include "map.h"
#include "battle_screen.h"
#include "config.h"
#include "highscore.h"
#include "item.h"
#include "inventory.h"
#include "generation.h"


void map_refresh(int player_y, int player_x);
void boss_map_refresh(int player_refresh_y, int player_refresh_x);
void handle_menu_input(int user_input);
void handle_map_input(int user_input);
int handle_battle_input(int user_input);
void handle_item_input(int user_input);
int handle_inventory_input(int user_input);
void reset_inventory(void);

//player/enemy position variables
int player_y = 1;
int player_x = 1;
int next_y = 0;
int next_x = 0;
int target_enemy_y;
int target_enemy_x;
int target_item_y;
int target_item_x;

//for highscore
int score = 0;

//for items and inventory
int inventory_track = 0;
items current_item;
items inventory[INVENTORY_SIZE];

//for states
    //for gamestate changes
gamestate current_gamestate = STATE_MENU;
    //mainly for debug and boss
mapstate current_mapstate = STATE_STANDARD;

int main(void)
{
    //for random map gen
    srand(time(NULL));

    //setup for screen/ncurses
    initscr();
    cbreak();
    noecho();

    //sets the map array up
    map_init(current_mapstate);
    visible_map_init(1, 1);
    
    //first menu init
    menu_init();
    menu_draw();

    //set up blank inventory
    reset_inventory();

    //main loop
    int user_input;
    while ((user_input = getch()) != 'q')
    {

        static int selection = 0;

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
                break;

            case STATE_INVENTORY:
                selection = handle_inventory_input(user_input);
                break;

            case STATE_ITEM:
                handle_item_input(user_input);
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
                break;

            case STATE_ITEM:
                item_screen_draw(current_item);
                break;

            case STATE_INVENTORY:
                inventory_screen_draw(selection, inventory);
        }

        refresh();
    }

    endwin();

    return 0;
}

//refreshes the game (duh)
void map_refresh(int player_refresh_y, int player_refresh_x)
{
    clear();
    map_draw(current_mapstate);
    mvaddch(player_refresh_y, player_refresh_x, '@');
    refresh();
}

void handle_menu_input(int user_input)
{
    menu_init();
    switch(user_input) 
    {
        case '1':  //regular mode
            map_init(current_mapstate);       // generate a new room
            reset_stats();    // reset player hp
            player_y = 1;     // reset player position
            player_x = 1;
            current_gamestate = STATE_MAP; 
            break;
        case '2': //debug mode
            current_mapstate = STATE_DEBUG;
            map_init(current_mapstate);
            reset_stats();
            player_y = 1;
            player_x = 1;
            current_gamestate = STATE_MAP; 


    }
}

void handle_map_input(int user_input)
{
    int boss_room_check;
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

    enemy_pursuit(player_y, player_x);

    //door collision check
    if ((boss_room_check = map_is_door(next_y, next_x)))
    {
        //door logic doubles as boss trigger logic
        switch (boss_room_check)
        {
            case BOSS_ROOM_ACTIVATION_COUNT: 
                current_mapstate = STATE_BOSS;
                map_init(current_mapstate);
                player_y = 1;
                player_x = 1;
                break;
            default:
                map_init(current_mapstate);
                player_y = 1;
                player_x = 1;
                break;
        }
        
    }
    //player-enemy collision check
    else if(map_is_enemy(next_y, next_x))
    {
        target_enemy_y = next_y;
        target_enemy_x = next_x;
        current_gamestate = STATE_BATTLE;
        battle_screen_init();
        battle_init(target_enemy_y, target_enemy_x);
        score = 0; //reset score
    }
    //player-item collision check
    else if(map_is_item(next_y, next_x))
    {
        target_item_y = next_y;
        target_item_x = next_x;
        current_item = random_item(); //generate item every time a player collides with an item
        current_gamestate = STATE_ITEM;
        init_item_screen();
    }
    //player-wall collision check
    else if (!map_is_wall(next_y, next_x))
    {
        player_y = next_y;
        player_x = next_x;
    }
    
    visible_map_init(player_y, player_x);
}

int handle_battle_input(int user_input)
{
    static choice selection = ATTACK;
    int lock = 0;

    //check for user input
    switch(user_input) 
    {
        case '1': selection = ATTACK; break;
        case '2': selection = INVENTORY; break;
        case '3': selection = RUN; break;
        case ENTER : lock = ENTER; break;
    }

    static battle_result outcome = BATTLE_IN_PROGRESS;

    //main battle logic function
    outcome = process_battle_turn(selection, lock, target_enemy_x, target_enemy_y);

    //reset lock variable (makes the game less buggy)
    lock = 0;

        

    switch (outcome)
    {
        case BATTLE_VICTORY:
            switch (current_mapstate)
            {
                case STATE_BOSS:
                //reset room counter to 0 so it can start over again 
                    int *room_counter = NULL;
                    room_counter = position_of_room_counter();
                    *room_counter = 0;
                    score = score_tracking(SCORE_FOR_DEFEATING_BOSS);
                    break;

                default: 
                    score = score_tracking(SCORE_FOR_DEFEATING_ENEMY);
            }
            //reset everything properly (MIGHT NEED TO CHANGE A BIT BECAUSE OF DEBUG STATE (PREVIOUS GAMESTATES))
            current_mapstate = STATE_STANDARD;
            current_gamestate = STATE_MAP;
            break;


        case BATTLE_DEFEAT: 
            //reset to 0
            int *room_counter = NULL;
            room_counter = position_of_room_counter();
            *room_counter = 0;
            //set (new) highscore
            score_register();
            //wipe everything
            reset_inventory();

            //same as in battle victory
            current_mapstate = STATE_STANDARD;
            current_gamestate = STATE_MENU;
            break;

        case BATTLE_FLED: 
            switch(current_mapstate)
            {
                case STATE_BOSS:
                    break;
                default:
                    current_gamestate = STATE_MAP;
            }
            break;
        case BATTLE_INVENTORY: 
            current_gamestate = STATE_INVENTORY;
            init_inventory_screen();
            break;
        default:
            break;
    }
    return selection;
}

void handle_item_input(int user_input)
{
    int *max_hp_manipulator = NULL;
    int *max_strength_manipulator = NULL;

    switch(user_input)
    {
        case ENTER :
            switch (current_item)
            {
                case HEAL:
                case POISON:
                case DAMAGE:
                    inventory[inventory_track] = current_item;
                    inventory_track++;
                    if (inventory_track == 10)
                    {
                        inventory_track = 0;
                    }
                    break;

                case EXTRA_STRENGTH: 
                    max_strength_manipulator = get_location_of(EXTRA_STRENGTH);
                    *max_strength_manipulator += EXTRA_STRENGTH_AMOUNT;
                    reset_stats();
                    break;

                case EXTRA_HP: 
                    max_hp_manipulator = get_location_of(EXTRA_HP);
                    *max_hp_manipulator += EXTRA_HP_AMOUNT;
                    reset_stats();
                    break;
                default: 
                    break;
            }
            break;
        case ESC :
            break;
        default: 
            return;
    }

    map_remove_item_at(target_item_y, target_item_x);
    current_gamestate = STATE_MAP;
    
}

int handle_inventory_input(int user_input)
{
    static int selection = 0;
    int lock = 0;

    switch(user_input) 
    {
        case 'd': selection++; break;
        case 'a': selection--; break;
        case ENTER: lock = ENTER; break;
        case ESC: lock = ESC; break;
    }

    if (selection < 0)
    {
        selection = INVENTORY_SIZE - 1;
    }
    else if (selection > INVENTORY_SIZE)
    {
        selection = 0;
    }

    if (lock == ENTER && inventory[selection] != EMPTY)
    {
        switch (inventory[selection])
        {
            case HEAL : 
                heal_potion();
                inventory[selection] = EMPTY;
                break;
            case DAMAGE :
                damage_potion();
                inventory[selection] = EMPTY;
                break;

            case POISON : 
                poison_enabler(true);
                inventory[selection] = EMPTY;
                break;

            default : break;
        }
        current_gamestate = STATE_BATTLE;
    }
    else if (lock == ESC)
    {
        current_gamestate = STATE_BATTLE;
    }
    return selection;
}

void reset_inventory(void)
{
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        inventory[i] = EMPTY;
    }
}