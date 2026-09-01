#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include "npc.h"
#include "generation.h"
#include "entity.h"


npc npc_init(int map_width, int map_height, int number, bool state)
{
    npc enemy;
    
    // npc creation (everywhere but 1|1)
    do {
        enemy.npc_x = get_random_int(1, map_width - 2);
        enemy.npc_y = get_random_int(1, map_height - 2);
    } while (enemy.npc_x == 1 && enemy.npc_y == 1);

    enemy.number = number;
    enemy.active = state;

    //dynamic variable names or something of same value

    //what the fuck does that mean kobe bryant? (Keeping this because i am gen interested in what i meant with this)
    return enemy;
}

entity standard_enemy_stats(int max, int current, int power)
{
    entity standard_enemy;

    standard_enemy.max_hp = max;
    standard_enemy.current_hp = current;
    standard_enemy.attack_power = power;

    return standard_enemy;
}


void standard_enemy_sprite(void)
{
    //main enemy (pointer sprite)
    mvprintw(2, 59, "\\  |  /");
    mvprintw(3, 60, "\\ | /");
    mvprintw(4, 57, "---- * ----");
    mvprintw(5, 60, "/ | \\");
    mvprintw(6, 59, "/  |  \\"); 
}