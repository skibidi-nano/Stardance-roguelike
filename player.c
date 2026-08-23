#include <ncurses.h>
#include "player.h"
#include "entity.h"

entity player_stats(int max, int current, int power)
{
    entity player;

    player.max_hp = max;
    player.current_hp = current;
    player.attack_power = power;

    return player;
}

void standard_player_sprite(void)
{
    //player sprite
    mvprintw(12, 9, ".-----.");
    mvprintw(13, 9, "/ .---. \\");
    mvprintw(14, 8, "| /  .-. | |");
    mvprintw(15, 8, "| |  | | | |");
    mvprintw(16, 9, "\\ \\  `'-' /");
    mvprintw(17, 10, "'\\____.-'"); 
}

