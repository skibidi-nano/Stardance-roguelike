#include "map.h"
#include <ncurses.h>
#include "config.h"

void map_draw(void)
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            if (y == 0 || y == MAP_HEIGHT -1 || x == 0 || x == MAP_WIDTH -1)
            {
                mvaddch(y, x, '#');
            }
        }
    }
}