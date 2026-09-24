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
#include "battle_log.h"

static char battle_screen[BATTLE_SCREEN_HEIGHT][BATTLE_SCREEN_WIDTH];
static char log_arr[BATTLE_LOG_HEIGHT][BATTLE_LOG_WIDTH];
static int max_hp = PLAYER_MAX_HP;
static int attack_power = PLAYER_ATTACK_DAMAGE;

static char enemy_type;
static entity *player_ptr;
static entity *enemy_ptr;

// initial stats entity
static entity player = { .max_hp = PLAYER_MAX_HP, .current_hp = 20, .attack_power = PLAYER_ATTACK_DAMAGE };
static entity enemy[LAST_ENUM + 1][MAX_NUMBER_OF_NPCS];
static type entity_type; 
static int entity_number;


static int enemy_turn_counter = 0; //for poison potion

static bool poison = false;
static bool boss_item = false;


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
        if (enemy_ptr->current_hp <= (player_ptr->attack_power)
        && value_of_current_mapstate() != STATE_BOSS) // if hp is lower than 10% of the max hp
        {
            int chance = get_random_int(1, 10);
            if (chance == 1)
            {
                current_turn = TURN_PLAYER;
                npc *room_enemies;
                room_enemies = position_of_enemy_array();
                room_enemies[entity_number].direction = false;
                return BATTLE_FLED;
            }
        }

        if (!boss_item) //if boss item is activated the enemy cant attack
        {
            // Enemy attacks player
            int enemy_damage = enemy_ptr->attack_power * get_random_int(1, 3); //for random damage
            player_ptr->current_hp -=  enemy_damage;

            battle_log(ENEMY_ATTACK, NO_DAMAGE_INPUT, log_arr);
            battle_log(PLAYER_DAMAGE_TOOK, enemy_damage, log_arr);

            if (player_ptr->current_hp <= 0)
            {
                reset_stats();
                return BATTLE_DEFEAT;
            }
        }
        else
        {
            static int boss_item_counter = 0;
            boss_item_counter++;
            if (boss_item_counter == 2)
            {
                boss_item = false;
            }
        }

        
        if(poison)
        {
            poison_call(enemy_turn_counter);
            battle_log(POISON_POTION_TICK, POISON_AMOUNT, log_arr);
            if (enemy_ptr->current_hp <= 0)
            {
                map_remove_enemy_at(enemy_y, enemy_x); //killing the enemy
                reset_stats();
                return BATTLE_VICTORY;
            }
        }

        enemy_turn_counter++;

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

                    battle_log(PLAYER_ATTACK, NO_DAMAGE_INPUT, log_arr);
                    battle_log(ENEMY_DAMAGE_TOOK, player_ptr->attack_power, log_arr);

                    //player_turn_counter++;
                    current_turn = TURN_ENEMY; // Switch turn
                    break;

                case INVENTORY: //self explandatory
                    return BATTLE_INVENTORY;
                    break;

                case RUN: //based on chance if you can escape, if you cant turn changes
                    int n = get_random_int(1, 3);
                    if(!(n % 2))
                    {
                        return BATTLE_FLED;
                    }
                    else
                    {
                        //player_turn_counter++;
                        current_turn = TURN_ENEMY;
                    }
            }
    }

    return BATTLE_IN_PROGRESS;
}


void battle_init(int enemy_y, int enemy_x)
{
    entity_number = search_for_enemy();
    player = player_stats(player.max_hp, player.current_hp, player.attack_power);
    enemy_type = value_of_part_of_map(enemy_y, enemy_x);
        switch (enemy_type)
        {
            case '&': entity_type = ADDRESS; break;
            case '%': entity_type = MODULO; break;
            case '*': entity_type = BOSS; break;
        }

        stats_enemy();
        player_ptr = &player; 
        enemy_ptr = &enemy[entity_type][entity_number];
}


//fills the battle array
void battle_screen_init(void)
{
    battle_log_init();

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

void battle_log_init(void)
{
    for (int y = 0; y < BATTLE_LOG_HEIGHT; y++)
    {
        for (int x = 0; x < BATTLE_LOG_WIDTH; x++)
        {
            if (y == 0 || y == BATTLE_LOG_HEIGHT -1 || x == 0 || x == BATTLE_LOG_WIDTH -1)
            {
                log_arr[y][x] = '#';
            }
            else 
            {
                log_arr[y][x] = ' ';
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

    draw_health_bar(player.current_hp / 5, player.max_hp / 5, HEALTH_BAR_POSITION_PLAYER);

    draw_current_hp(player.current_hp, HEALTH_BAR_POSITION_PLAYER + 1);

    draw_current_strength(player.attack_power, HEALTH_BAR_POSITION_PLAYER + 1);

    standard_player_sprite();




    //Enemy set up
        //Enemy health bar

    draw_health_bar(enemy[entity_type][entity_number].current_hp / 5, enemy[entity_type][entity_number].max_hp / 5, HEALTH_BAR_POSITION_ENEMY);

    draw_current_hp(enemy[entity_type][entity_number].current_hp, HEALTH_BAR_POSITION_ENEMY + 1);

    draw_current_strength(enemy[entity_type][entity_number].attack_power, HEALTH_BAR_POSITION_ENEMY + 1);

    switch (entity_type)
    {
        case ADDRESS: address_enemy_sprite(); break;
        case MODULO: modulo_enemy_sprite(); break;
        case BOSS: boss_sprite();
        default: break;
    }
    


    //draw menu
    battle_menu_draw(selection);

    for (int y = 0; y < BATTLE_LOG_HEIGHT; y++)
    {
        for (int x = 0; x < BATTLE_LOG_WIDTH; x++)
        {
            if (log_arr[y][x] != '\0') //fix for ^ symbol in the logs
            {
                mvaddch(y + BATTLE_LOG_POSITION_Y_CURRENT, x + BATTLE_LOG_POSITION_X, log_arr[y][x]);
            }
        }
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

void reset_stats(void)
{
    player.max_hp = max_hp;
    player.current_hp = player.max_hp; 
    player.attack_power = attack_power;

    enemy[entity_type][entity_number].max_hp = 0;
    enemy[entity_type][entity_number].current_hp = 0;
    enemy[entity_type][entity_number].attack_power = 0;
}

void stats_enemy(void)
{

    switch (entity_type)
    {
        case ADDRESS: //stats for standard enemies
            enemy[ADDRESS][entity_number].max_hp = 80 + (4 * value_of_boss_counter());            
            enemy[ADDRESS][entity_number].attack_power = 9 + (2 * value_of_boss_counter());
            break;
        case MODULO: //stats for special enemies
            enemy[MODULO][entity_number].max_hp = 75 + (3 * value_of_boss_counter());
            enemy[MODULO][entity_number].attack_power = 12 + (2 * value_of_boss_counter());
            break;
        case BOSS:
            enemy[BOSS][entity_number].max_hp = 120 + (6 * value_of_boss_counter());
            enemy[BOSS][entity_number].attack_power = 15 + (2 * value_of_boss_counter());
            break;
        default:
            break;


    }

    if (enemy[entity_type][entity_number].current_hp == 0)
    {
        enemy[entity_type][entity_number].current_hp = enemy[entity_type][entity_number].max_hp;
    }
}

int search_for_enemy(void)
{
    player_map player_pos;
    player_pos = value_of_player_pos();
    npc *enemies = NULL;
    enemies = position_of_enemy_array();

    for (int i = 0; i < MAX_NUMBER_OF_NPCS; i++)
    {
        if (enemies[i].npc_x == player_pos.target_enemy_x && enemies[i].npc_y == player_pos.target_enemy_y)
        {
            return enemies[i].number;
        }
    }

    return LAST_ENUM;
}

int* get_location_of(items item) //need to rewrite    ps:not sure tho probably just a name change
{
    switch (item)
    {
        case HEAL: return &player.current_hp;
        case EXTRA_STRENGTH: return &attack_power;
        case EXTRA_HP: return &max_hp;
        case DAMAGE: return &enemy[entity_type][entity_number].current_hp;
        default: return NULL;
    }
}

bool* get_location_of_poison(void)
{
    return &poison;
}

bool* get_location_of_boss_bool(void)
{
    return &boss_item;
}

entity (*get_location_of_enemies(void))[MAX_NUMBER_OF_NPCS] {
    return enemy;
}

void call_battle_log(actions input_one, int input_two)
{
    battle_log(input_one, input_two, log_arr);
}