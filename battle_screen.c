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
static int attack_power = 5;

static char enemy_type;
static entity *player_ptr;
static entity *enemy_ptr;
// initial stats entity
static entity player = { .max_hp = 20, .current_hp = 20, .attack_power = 5 };
static entity enemy;  
//sets the stats for the entities that are fighting

battle_result process_battle_turn(choice selection, int lock, int enemy_x, int enemy_y)
{
    //static so they store the same addresses across loops

    //npc *enemies = position_of_enemy_array(); POTENTIALLY USEFULL CURRENTLY USELESS
    static turn_player current_turn = TURN_PLAYER;

    //check for issues
    if (player_ptr == NULL || enemy_ptr == NULL)
    {
        return BATTLE_ERROR;
    }

    //ENEMY LOGIC
    if (current_turn == TURN_ENEMY)
    {
        // Enemy attacks player
        player_ptr->current_hp -= enemy_ptr->attack_power * get_random_int(1, 3); //for random damage
        if (player_ptr->current_hp <= 0)
        {
            reset_stats();
            return BATTLE_DEFEAT;
        }
        
        current_turn = TURN_PLAYER; // Switch turn
        return BATTLE_IN_PROGRESS;
    }

    //PLAYER LOGIC
    if (current_turn == TURN_PLAYER && lock == ENTER)
    {
        switch (selection)
            {
                case ATTACK:
                    enemy_ptr->current_hp -= player_ptr->attack_power;
                    if (enemy_ptr->current_hp <= 0)
                    {
                        map_remove_enemy_at(enemy_y, enemy_x); //killing the enemy
                        reset_stats();
                        return BATTLE_VICTORY;
                    }
                    current_turn = TURN_ENEMY; // Switch turn
                    break;

                case INVENTORY: //self explandatory
                    return BATTLE_INVENTORY;
                    break;

                case RUN: //based on chance if you can escape, if you cant you take damage
                    int n = get_random_int(1, 3);
                    if(!(n % 2))
                    {
                    return BATTLE_FLED;
                    }
                    else
                    {
                        current_turn = TURN_ENEMY;
                    }
            }
    }

    return BATTLE_IN_PROGRESS;
}


void battle_init(int enemy_y, int enemy_x)
{

    player = player_stats(player.max_hp, player.current_hp, player.attack_power);
    enemy_type = value_of_part_of_map(enemy_y, enemy_x);
        switch (enemy_type)
        {
            case '&': enemy = stats_enemy(ADDRESS); break;
            case '%': enemy = stats_enemy(MODULO); break;
            case '*': enemy = stats_enemy(BOSS); break;
        }
        player_ptr = &player; 
        enemy_ptr = &enemy;
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

    draw_health_bar(player.current_hp, player.max_hp, HEALTH_BAR_POSITION_PLAYER);

    draw_current_hp(player.current_hp, HEALTH_BAR_POSITION_PLAYER + 1);

    draw_current_strength(player.attack_power, HEALTH_BAR_POSITION_PLAYER + 1);

    standard_player_sprite();




    //Enemy set up
        //Enemy health bar

    draw_health_bar(enemy.current_hp, enemy.max_hp, HEALTH_BAR_POSITION_ENEMY);

    draw_current_hp(enemy.current_hp, HEALTH_BAR_POSITION_ENEMY + 1);

    draw_current_strength(enemy.attack_power, HEALTH_BAR_POSITION_ENEMY + 1);

    switch (enemy_type)
    {
        case '&': address_enemy_sprite(); break;
        case '%': modulo_enemy_sprite(); break;
        case '*': boss_sprite();
    }
    


    //draw menu
    if (selection == ATTACK)
    {
        battle_menu_draw(selection);
    }
    else if(selection == INVENTORY)
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
    if (selection == ATTACK) mvprintw(13, 55, " > [1] ATTACK");
    else mvprintw(13, 55, " [1] ATTACK");

    if (selection == INVENTORY) mvprintw(15, 55, " > [2] INVENTORY");
    else mvprintw(15, 55, " [2] INVENTORY");

    if (selection == RUN) mvprintw(17, 55, " > [3] RUN");
    else mvprintw(17, 55, " [3] RUN");
}

int* get_location_of(items item) //need to rewrite    ps:not sure tho probably just a name change
{
    switch (item)
    {
        case HEAL: return &player.current_hp;
        case EXTRA_STRENGTH: return &attack_power;
        case EXTRA_HP: return &max_hp;
        case DAMAGE: return &enemy.current_hp;
        default: return NULL;
    }
}

void reset_stats(void)
{
    player.max_hp = max_hp;
    player.current_hp = player.max_hp; 
    player.attack_power = attack_power;

    enemy.max_hp = 20;
    enemy.current_hp = 20;
    enemy.attack_power = 3;
}

entity stats_enemy(type type)
{
    entity current_enemy;
    switch (type)
    {
        case ADDRESS: //stats for standard enemies
            current_enemy.max_hp = 20 + value_of_boss_counter();
            current_enemy.current_hp = current_enemy.max_hp;
            current_enemy.attack_power = 3 + (2 * value_of_boss_counter());
            current_enemy.type = ADDRESS;
            break;
        case MODULO: //stats for special enemies
            current_enemy.max_hp = 15 + value_of_boss_counter();
            current_enemy.current_hp = current_enemy.max_hp;
            current_enemy.attack_power = 4 + (2 * value_of_boss_counter());
            current_enemy.type = MODULO;
            break;
        case BOSS:
            current_enemy.max_hp = 25 + (2 * value_of_boss_counter());
            current_enemy.current_hp = current_enemy.max_hp;
            current_enemy.attack_power = 2 * value_of_boss_counter();
            current_enemy.type = BOSS;


    }

    return current_enemy;
}