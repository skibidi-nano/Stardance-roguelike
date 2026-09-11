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


void map_refresh(int player_y, int player_x);
void boss_map_refresh(int player_refresh_y, int player_refresh_x);
void handle_menu_input(int user_input);
void handle_map_input(int user_input);
int handle_battle_input(int user_input);
void handle_item_input(int user_input);
int handle_inventory_input(int user_input);
void reset_inventory(void);

//player starting position (prob will be changed)
int player_y = 1;
int player_x = 1;
int next_y = 0;
int next_x = 0;
int target_enemy_y;
int target_enemy_x;
int target_item_y;
int target_item_x;


int battle_check = 0;
int score = 0;

int inventory_track = 0;
items current_item;
items inventory[INVENTORY_SIZE];

gamestate current_gamestate = STATE_MENU;
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
    
    //first menu init
    menu_init();
    menu_draw();

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
                if (current_gamestate != STATE_BATTLE)
                { 
                    break;
                }
                else
                {
                    user_input = 0;
                }

            case STATE_BATTLE:
                selection = handle_battle_input(user_input);
                if (current_gamestate != STATE_INVENTORY)
                { 
                    break;
                }
                else
                {
                    user_input = 0;
                }

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
        case '1':  //regular mode
            map_init(current_mapstate);       // generate a new room
            reset_stats();    // reset player hp
            player_y = 1;     // reset player position
            player_x = 1;
            current_gamestate = STATE_MAP; 
            break;
        case '2': //debug mode
            current_mapstate = STATE_DEBUG;
            map_init(current_mapstate);       // generate a new room
            reset_stats();    // reset player hp
            player_y = 1;     // reset player position
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

    //door collision check
    if ((boss_room_check = map_is_door(next_y, next_x)))
    {
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
        current_gamestate = STATE_BATTLE;
        battle_check = 0;
        target_enemy_y = next_y;
        target_enemy_x = next_x;
    }
    //player-item collision check
    else if(map_is_item(next_y, next_x))
    {
        target_item_y = next_y;
        target_item_x = next_x;
        current_item = random_item(); //generate item every time a player collides with an item
        current_gamestate = STATE_ITEM;
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
        battle_init(target_enemy_y, target_enemy_x);
        score = 0; //reset score
        battle_check++;
        return 0;
    }

    //check for user input
    switch(user_input) 
    {
        case '1': selection = ATTACK; break;
        case '2': selection = INVENTORY; break;
        case '3': selection = RUN; break;
        case ENTER : lock = ENTER; break;
    }
    
    if (selection == INVENTORY && lock == ENTER)
    {
        current_gamestate = STATE_INVENTORY;
        return selection;
    }

    static battle_result outcome = BATTLE_IN_PROGRESS;

    outcome = process_battle_turn(battle_check, selection, lock, target_enemy_x, target_enemy_y);

    lock = 0;

    if (outcome == BATTLE_VICTORY)
    {
        score = score_tracking(SCORE_FOR_DEFEATING_ENEMY); 
        /*int *room_counter = NULL; ////////////////////////////////////
        room_counter = position_of_room_counter(); ///////////////////
        *room_counter = 0; //////////////////////////////////TEMPORARY*/
        current_mapstate = STATE_STANDARD;
        current_gamestate = STATE_MAP;
    }
    else if (outcome == BATTLE_DEFEAT)
    {
        int *room_counter = NULL;
        room_counter = position_of_room_counter();
        *room_counter = 0;
        score_register();
        reset_inventory();
        current_mapstate = STATE_STANDARD;
        current_gamestate = STATE_MENU;
    }

    return selection;
}

void handle_item_input(int user_input)
{
    int lock = 0;
    int *max_hp_manipulator = NULL;
    int *max_strength_manipulator = NULL;

    init_item_screen();

    switch(user_input)
    {
        case ENTER : lock = ENTER; break;
        case ESC : lock = ESC;
    }
    if (lock == ENTER)
    {
        switch (current_item)
        {
            case HEAL:
                    inventory[inventory_track] = current_item;
                    inventory_track++;
                    if (inventory_track == 10)
                    {
                        inventory_track = 0;
                    }
                    break;
        
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
            default: break;
        }

        map_remove_item_at(target_item_y, target_item_x);
        current_gamestate = STATE_MAP;
    }
    else if (lock == ESC)
    {
        map_remove_item_at(target_item_y, target_item_x);

        current_gamestate = STATE_MAP;
    }

    
}

int handle_inventory_input(int user_input)
{
    static int selection = 0;
    int lock = 0;
    int *heal_manipulator = NULL;
    int *max_hp_ptr = NULL;
    
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

            default : break;
        }
        current_gamestate = STATE_BATTLE;
    }
    else if (lock == ESC)
    {
        current_gamestate = STATE_BATTLE;
    }

    init_inventory_screen();

    return selection;
}

void reset_inventory(void)
{
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        inventory[i] = EMPTY;
    }
}