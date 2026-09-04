#include <ncurses.h>
#include "generation.h"
#include "battle_screen.h"
#include "config.h"
#include "npc.h"
#include "player.h"
#include "entity.h"
#include "map.h"
#include "npc.h"
#include "item.h"

static char battle_screen[BATTLE_SCREEN_HEIGHT][BATTLE_SCREEN_WIDTH];
static int max_hp = 20;
static int current_hp = 20;
static int attack_power = 5;

// initial stats entity
static entity player = { .max_hp = 20, .current_hp = 20, .attack_power = 5 };
static entity enemy  = { .max_hp = 20, .current_hp = 20, .attack_power = 3 };
//sets the stats for the entities that are fighting

battle_result process_battle_turn(int init_mode, choice selection, int lock, int enemy_x, int enemy_y)
{
    //static so they store the same addresses across loops
    static entity *player_ptr = NULL;
    static entity *enemy_ptr = NULL;
    npc *enemies = position_of_enemy_array();
    static turn_player current_turn = TURN_PLAYER;

    //gets called one to set up the entities
    if (init_mode == 0)
    {
        player_ptr = &player;
        enemy_ptr = &enemy;
        current_turn = TURN_PLAYER; //default starting turn is player
        return BATTLE_SETUP;
    }

    //check for issues
    if (player_ptr == NULL || enemy_ptr == NULL)
    {
        return BATTLE_ERROR;
    }

    //PLAYER TURN
    if (current_turn == TURN_PLAYER && selection == ATTACK && lock == ENTER)
    {
        // Player attacks enemy (dereferencing the pointer to modify values)
        enemy_ptr->current_hp -= player_ptr->attack_power;
        if (enemy_ptr->current_hp < 0) 
        {    
            enemy_ptr->current_hp = 0;
        }
        if (enemy_ptr->current_hp == 0)
        {
            map_remove_enemy_at(enemy_y, enemy_x);
            reset_stats();
            return BATTLE_VICTORY;
        }
        
        current_turn = TURN_ENEMY; // Switch turn
    }
    else if (current_turn == TURN_PLAYER && selection == RUN && lock == ENTER)
    {
        //temporary solution, will be implemented soon
        endwin();
        return BATTLE_VICTORY;
    }
    //ENEMY TURN
    else if (current_turn == TURN_ENEMY)
    {
        // Enemy attacks player
        player_ptr->current_hp -= enemy_ptr->attack_power * get_random_int(1, 3); //for random damage
        if (player_ptr->current_hp < 0)
        {
            player_ptr->current_hp = 0;
        }
        if (player_ptr->current_hp == 0)
        {
            reset_stats();
            return BATTLE_DEFEAT;
        }
        

        current_turn = TURN_PLAYER; // Switch turn
    }

    return BATTLE_IN_PROGRESS;
}


void battle_init(void)
{
    player = player_stats(player.max_hp, player.current_hp, player.attack_power);
    enemy = standard_enemy_stats(enemy.max_hp, enemy.current_hp, enemy.attack_power);
    turn_player current_turn = TURN_ENEMY;
    
    battle_result init = process_battle_turn(0, 0, 0, 0, 0);
}


//fills the battle array
void battle_screen_init(void)
{
    for (int y = 0; y < BATTLE_SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < BATTLE_SCREEN_WIDTH; x++)
        {
            if (y == 0 || y == BATTLE_SCREEN_HEIGHT -1 || x == 0 || x == BATTLE_SCREEN_WIDTH -1)
            {
                battle_screen[y][x] = '#';
            }
            else 
            {
                battle_screen[y][x] = ' ';
            }
        }
    }
}



void battle_screen_draw(choice selection)
{
    //set the stage up
    for (int y = 0; y < BATTLE_SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < BATTLE_SCREEN_WIDTH; x++)
        {
            mvaddch(y, x, battle_screen[y][x]);
        }
    }

    //Player set up
        //Player health bar
    int current_health_player = health_bar_init(player.current_hp, player.max_hp);
    draw_health_bar(current_health_player, player.max_hp, HEALTH_BAR_POSITION_PLAYER);
    standard_player_sprite();

    //Enemy set up
        //Enemy health bar
    int current_health_enemy = health_bar_init(enemy.current_hp, enemy.max_hp);
    draw_health_bar(current_health_enemy, enemy.max_hp, HEALTH_BAR_POSITION_ENEMY);
    standard_enemy_sprite();

    //draw menu
    if (selection == ATTACK)
    {
        battle_menu_draw(selection);
    }
    else if(selection == RUN)
    {
        battle_menu_draw(selection);
    }
    
}

void battle_menu_draw(choice selection)
{
    if (selection == ATTACK) mvprintw(15, 55, "> ATTACK");
    else mvprintw(15, 55, "  ATTACK");

    if (selection == RUN) mvprintw(17, 55, "> RUN");
    else mvprintw(17, 55, "  RUN");
}

int* get_location_of(items item)
{
    switch (item)
    {
        case HEAL: return &current_hp;
        case EXTRA_STRENGTH: return &attack_power;
        case EXTRA_HP: return &max_hp;
        default: return NULL;
    }
}

void reset_stats(void)
{
    player.max_hp = max_hp;
    player.current_hp = 20; 
    player.attack_power = attack_power;

    enemy.max_hp = 20;
    enemy.current_hp = 20;
    enemy.attack_power = 3;
}
