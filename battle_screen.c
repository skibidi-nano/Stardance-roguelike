#include <ncurses.h>
#include "generation.h"
#include "battle_screen.h"
#include "config.h"
#include "npc.h"
#include "player.h"
#include "entity.h"

static char battle_screen[BATTLE_SCREEN_HEIGHT][BATTLE_SCREEN_WIDTH];

// initial stats entity
static entity player = { .max_hp = 20, .current_hp = 20, .attack_power = 5 };
static entity enemy  = { .max_hp = 20, .current_hp = 20, .attack_power = 3 };
//sets the stats for the entities that are fighting

int process_battle_turn(int init_mode, int selection, int lock)
{
    //static so they store the same addresses across loops
    static entity *player_ptr = NULL;
    static entity *enemy_ptr = NULL;
    static turn_player current_turn = TURN_PLAYER;

    //gets called one to set up the entities
    if (init_mode == 0)
    {
        player_ptr = &player;
        enemy_ptr = &enemy;
        current_turn = TURN_PLAYER; //default starting turn is player
        return 3;
    }

    //check for issues
    if (player_ptr == NULL || enemy_ptr == NULL)
    {
        return 4;
    }

    if (selection == 0)
    {
        battle_menu_draw(selection = 0);
    }
    else if (selection > 1)
    {
        battle_menu_draw(selection = 0);
    }
    else if(selection == 1)
    {
        battle_menu_draw(selection = 1);
    }
    else if (selection < 0)
    {
        battle_menu_draw(selection = 1);
    }
    


    //PLAYER TURN
    if (current_turn == TURN_PLAYER && selection == 1 && lock == ENTER)
    {
        // Player attacks enemy (dereferencing the pointer to modify values)
        enemy_ptr->current_hp -= player_ptr->attack_power;
        if (enemy_ptr->current_hp < 0) 
        {    
            enemy_ptr->current_hp = 0;
        }
        if (enemy_ptr->current_hp == 0)
        {
            return 1;
        }
        
        current_turn = TURN_ENEMY; // Switch turn
    }
    else if (current_turn == TURN_PLAYER && selection == 0 && lock == ENTER)
    {
        //temporary solution, will be implemented soon
        endwin();
        return 1;
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
            return 0;
        }
        
        current_turn = TURN_PLAYER; // Switch turn
    }

    return 5;
}
void battle_init(void)
{
    player = player_stats(player.max_hp, player.current_hp, player.attack_power);
    enemy = standard_enemy_stats(enemy.max_hp, enemy.current_hp, enemy.attack_power);
    turn_player current_turn = TURN_ENEMY;
    
    process_battle_turn(0, 0, 0);
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

void battle_screen_draw()
{
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
    draw_health_bar(current_health_player, player.max_hp);
    standard_player_sprite();

    //Enemy set up
        //Enemy health bar
    int current_health_enemy = health_bar_init(player.current_hp, player.max_hp);
    draw_health_bar(current_health_player, player.max_hp);
    standard_enemy_sprite();
    
}

void battle_menu_draw(int selection)
{
    if (selection == 0) mvprintw(15, 55, "> ATTACK");
    else mvprintw(15, 55, "  ATTACK");

    if (selection == 1) mvprintw(17, 55, "> RUN");
    else mvprintw(17, 55, "  RUN");
}
