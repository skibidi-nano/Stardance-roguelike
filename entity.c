#include <ncurses.h>
#include "config.h"
#include "entity.h"

void draw_health_bar(int current_hp, int max_hp, int y)
{
    for (int i = 0; i < max_hp; i++)
    {
       if (i < current_hp)
       {
            mvprintw(y, 2 + i, "+");
       }
       else
       {
            mvprintw(y, 2 + i, "-");
       }
    }
}


void draw_current_hp(int current_hp, int y)
{
    mvprintw(y, 2, "#HP");
    mvprintw(y, 7, "%i", current_hp);
}

void draw_current_strength(int current_strength, int y)
{
    mvprintw(y, 13, "#ATK");
    mvprintw(y, 18, "%i", current_strength);
}