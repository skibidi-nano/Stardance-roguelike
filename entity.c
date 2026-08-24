#include <ncurses.h>
#include "config.h"
#include "entity.h"

int health_bar_init(int current_hp, int max_hp)
{
    return (current_hp * max_hp) / max_hp;
}

void draw_health_bar(int health_bar, int max_hp)
{
    for (int i = 0; i < max_hp; i++)
    {
       if (i < health_bar)
       {
            mvprintw(2, 2 + i, "+");
       }
       else
       {
            mvprintw(2, 2 + i, "-");
       }
    }
}

