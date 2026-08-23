#include <ncurses.h>
#include "battle_screen.h"
#include "config.h"
#include "npc.h"
#include "player.h"
#include "entity.h"

static char battle_screen[BATTLE_SCREEN_HEIGHT][BATTLE_SCREEN_WIDTH];

// initial stats entity
entity player;
entity enemy;

player.attack_power = 5
player.max_hp = 20
player.current_hp = player.max_hp

enemy.attack_power = 3
enemy.max_hp = 20
enemy.current_hp = enemy.max_hp


//sets the stats for the entities that are fighting
void battle_init(void)
{
    player = player_stats(player.max_hp, player.current_hp, player.attack_power);
    enemy = standard_enemy_stats(enemy.max_hp, enemy.current_hp, enemy.attack_power);
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

void battle_screen_draw(void)
{
    for (int y = 0; y < BATTLE_SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < BATTLE_SCREEN_WIDTH; x++)
        {
            mvaddch(y, x, battle_screen[y][x]);
        }
    }

    char *life = "~~~~~~~~~";
    mvprintw(2, 2, "%s", life);
    standard_enemy_sprite();
    standard_player_sprite();
}