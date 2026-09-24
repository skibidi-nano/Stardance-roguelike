#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "menu.h"
#include "map.h"
#include "battle_screen.h"
#include "battle_log.h"
#include "config.h"
#include "highscore.h"
#include "item.h"
#include "inventory.h"
#include "generation.h"


void handle_menu_input(int user_input);
void controls_screen(void);
void handle_controls_screen_input(int user_input);
void handle_map_input(int user_input);
int handle_battle_input(int user_input);
void handle_item_input(int user_input);
int handle_inventory_input(int user_input);
void reset_inventory(void);
player_map value_of_player_pos(void);
mapstate value_of_current_mapstate(void);


//player/enemy position variables
player_map player_pos = {.y = 1, .x = 1, .next_y = 0, .next_x = 0,};

//for highscore
int score = 0;

//for items and inventory
bool inventory_full = false;
items current_item;
items inventory[INVENTORY_SIZE];

//for states
    //for gamestate changes
gamestate current_gamestate = STATE_MENU;
    //for handling which kind of map should be printed
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

        static selection selection = {.inventory = 0, .battle = 0};

        switch (current_gamestate)
        {
            case STATE_MENU:
                handle_menu_input(user_input);
                break;

            case STATE_MAP:
                handle_map_input(user_input);
                break;
            case STATE_BATTLE:
                selection.battle = handle_battle_input(user_input);
                break;

            case STATE_INVENTORY:
                selection.inventory = handle_inventory_input(user_input);
                break;

            case STATE_ITEM:
                handle_item_input(user_input);
                break;
            
            case STATE_CONTROLS:
                handle_controls_screen_input(user_input);
                break;
        }

        clear();

        switch (current_gamestate)
        {
            case STATE_MENU:
                menu_draw();
                break;

            case STATE_MAP:
                map_refresh(player_pos.y, player_pos.x, current_mapstate);
                break;

            case STATE_BATTLE:
                battle_screen_draw (selection.battle);
                break;

            case STATE_ITEM:
                item_screen_draw(current_item, inventory_full);
                break;

            case STATE_INVENTORY:
                inventory_screen_draw(selection.inventory, inventory);
                break;
            
            case STATE_CONTROLS:
                controls_screen();
        }

        refresh();
    }

    endwin();

    return 0;
}

void handle_menu_input(int user_input)
{
    menu_init();
    switch(user_input) 
    {
        case '1':  //regular mode
            map_init(current_mapstate);       // generate a new room
            reset_stats();    // reset player hp
            player_pos.y = 1;     // reset player position
            player_pos.x = 1;
            current_gamestate = STATE_MAP; 
            break;
        case '2': //debug mode
            current_mapstate = STATE_DEBUG;
            map_init(current_mapstate);
            reset_stats();
            player_pos.y = 1;
            player_pos.x = 1;
            current_gamestate = STATE_MAP; 
            break;
        case '3': //controls screen
            current_gamestate = STATE_CONTROLS;


    }
}

void controls_screen(void)
{
    mvprintw(1, 1, "Controls screen, press ESC to exit");
    mvprintw(2, 1, "MOVEMENT: WASD");
    mvprintw(3, 1, "QUIT GAME AT ANY TIME: q");
    mvprintw(4, 1, "BATTLE: 1, 2 OR 3, PRESS ENTER TO SELECT");
    mvprintw(5, 1, "INVENTORY: A OR D, PRESS ENTER TO SELECT");

}

void handle_controls_screen_input(int user_input)
{
    switch(user_input)
    {
        case ESC: current_gamestate = STATE_MENU; menu_init(); break;
        default: break;
    }
}

void handle_map_input(int user_input)
{
    int boss_room_check;
    player_pos.next_y = player_pos.y;
    player_pos.next_x = player_pos.x;

    //action based on input
    switch(user_input) 
    {
        case 'w': player_pos.next_y--; break;
        case 's': player_pos.next_y++; break;
        case 'a': player_pos.next_x--; break;
        case 'd': player_pos.next_x++; break;
    }

    enemy_pursuit(player_pos.y, player_pos.x, current_mapstate);

    //door collision check
    if ((boss_room_check = map_is_door(player_pos.next_y, player_pos.next_x)))
    {
        //door logic doubles as boss trigger logic
        switch (boss_room_check)
        {
            case BOSS_ROOM_ACTIVATION_COUNT: 
                current_mapstate = STATE_BOSS;
                map_init(current_mapstate);
                player_pos.y = 1;
                player_pos.x = 1;
                break;
            default:
                current_mapstate = STATE_STANDARD;
                map_init(current_mapstate);
                player_pos.y = 1;
                player_pos.x = 1;
                break;
        }
        
    }
    //player-enemy collision check
    else if(map_is_enemy(player_pos.next_y, player_pos.next_x))
    {
        player_pos.target_enemy_y = player_pos.next_y; ////////////////MIGHT NEED TO REVISIST
        player_pos.target_enemy_x = player_pos.next_x;
        current_gamestate = STATE_BATTLE;
        battle_screen_init();
        battle_init(player_pos.target_enemy_y, player_pos.target_enemy_x);
        score = 0; //reset score
    }
    //player-item collision check
    else if(map_is_item(player_pos.next_y, player_pos.next_x))
    {
        player_pos.target_item_y = player_pos.next_y;
        player_pos.target_item_x = player_pos.next_x;
        switch (current_mapstate)
        {
            case STATE_BOSS:
                current_item = BOSS_ITEM;
                break;
            default: 
                current_item = random_item();
        }
        current_gamestate = STATE_ITEM;
        init_item_screen();
    }
    //player-wall collision check
    else if (!map_is_wall(player_pos.next_y, player_pos.next_x))
    {
        player_pos.y = player_pos.next_y;
        player_pos.x = player_pos.next_x;
    }
    
    visible_map_init(player_pos.y, player_pos.x);
}

int handle_battle_input(int user_input)
{
    static choice battle_selection = ATTACK;
    int lock = 0;

    //check for user input
    switch(user_input) 
    {
        case '1': battle_selection = ATTACK; break;
        case '2': battle_selection = INVENTORY; break;
        case '3': battle_selection = RUN; break;
        case ENTER : lock = ENTER; break;
        default: battle_selection = ATTACK; break;
    }

    static battle_result outcome = BATTLE_IN_PROGRESS;

    //main battle logic function
    outcome = process_battle_turn(battle_selection, lock, player_pos.target_enemy_x, player_pos.target_enemy_y);

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
                    boss_item_creation();
                    break;

                default: 
                    score = score_tracking(SCORE_FOR_DEFEATING_ENEMY);
                    current_mapstate = STATE_STANDARD;
            }
            //reset everything properly
            current_gamestate = STATE_MAP;
            break;


        case BATTLE_DEFEAT: 
            //reset room counter to 0
            int *room_counter = NULL;
            room_counter = position_of_room_counter();
            *room_counter = 0;
            
            //reset boss counter to 0
            int *boss_counter = NULL;
            boss_counter = position_of_boss_counter();
            *boss_counter = 0;

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
    return battle_selection;
}

void handle_item_input(int user_input)
{
    int *max_hp_manipulator = NULL;
    int *max_strength_manipulator = NULL;
    pass check = ENTER_ILLEGAL;

    switch(user_input)
    {
        case ENTER :
            switch (current_item)
            {
                case HEAL:
                case DAMAGE:
                case POISON:
                case BOSS_ITEM:
                    check = assign_item_to_inventory(inventory, current_item);
                    break;

                case EXTRA_STRENGTH: 
                    max_strength_manipulator = get_location_of(EXTRA_STRENGTH);
                    *max_strength_manipulator += EXTRA_STRENGTH_AMOUNT;
                    reset_stats();
                    check = ENTER_LEGAL;
                    break;

                case EXTRA_HP: 
                    max_hp_manipulator = get_location_of(EXTRA_HP);
                    *max_hp_manipulator += EXTRA_HP_AMOUNT;
                    reset_stats();
                    check = ENTER_LEGAL;
                    break;

                default: 
                    break;
            }
            break;
        case ESC :
            check = ENTER_ESC;
            break;
        default: 
            return;
    }

    switch (check)
    {
        case ENTER_ESC:
        case ENTER_LEGAL:
            map_remove_item_at(player_pos.target_item_y, player_pos.target_item_x);
            visible_map_init(player_pos.y, player_pos.x);
            current_gamestate = STATE_MAP;
            break;
        
        case ENTER_ILLEGAL:
            inventory_full = true;
            break;
    }
    
    
}

int handle_inventory_input(int user_input)
{
    static int inventory_selection = 0;
    int lock = 0;

    switch(user_input) 
    {
        case 'd': inventory_selection++; break;
        case 'a': inventory_selection--; break;
        case ENTER: lock = ENTER; break;
        case ESC: lock = ESC; break;
    }

    if (inventory_selection < 0)
    {
        inventory_selection = INVENTORY_SIZE - 1;
    }
    else if (inventory_selection > INVENTORY_SIZE)
    {
        inventory_selection = 0;
    }

    if (lock == ENTER && inventory[inventory_selection] != EMPTY)
    {
        switch (inventory[inventory_selection])
        {
            case HEAL : 
                heal_potion();
                inventory[inventory_selection] = EMPTY;

                call_battle_log(PLAYER_HEAL_USE, NO_DAMAGE_INPUT);
                break;
            case DAMAGE :
                damage_potion();
                inventory[inventory_selection] = EMPTY;

                call_battle_log(PLAYER_DAMAGE_USE, NO_DAMAGE_INPUT);
                break;

            case POISON : 
                poison_enabler(true);
                inventory[inventory_selection] = EMPTY;

                call_battle_log(PLAYER_POISON_USE, NO_DAMAGE_INPUT);
                break;

            case BOSS_ITEM:
                bool *boss_item_enabler = NULL;
                boss_item_enabler = get_location_of_boss_bool();
                *boss_item_enabler = true;

                inventory[inventory_selection] = EMPTY;

                call_battle_log(PLAYER_BOSS_ITEM_USE, NO_DAMAGE_INPUT);
                break;

            default : break;   
        }
        inventory_full = false;
        current_gamestate = STATE_BATTLE;
    }
    else if (lock == ESC)
    {
        current_gamestate = STATE_BATTLE;
    }
    return inventory_selection;
}

void reset_inventory(void)
{
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        inventory[i] = EMPTY;
    }
}

player_map value_of_player_pos(void)
{
    return player_pos;
}

mapstate value_of_current_mapstate(void)
{
    return current_mapstate;
}