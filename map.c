#include "map.h"
#include <ncurses.h>

static char map[MAP_HEIGHT][MAP_WIDTH];

//initialisation of the map array
void map_init(void)
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            if (y == 0 || y == MAP_HEIGHT -1 || x == 0 || x == MAP_WIDTH -1)
            {
                map[y][x] = '#';
            }
            else
            {
                map[y][x] = ' ';
            }
        }
    }
}

//Puts the map on the screen
void map_draw(void)
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            mvaddch(y, x, map[y][x]);
        }
    }
}

//checks for walls
int map_is_wall(int y, int x)
{
    //everything outside of the map is treated as a wall
    if (y < 0 || y >= MAP_HEIGHT || x < 0 || x >= MAP_WIDTH)
    {
        return 1;
    }
    return map[y][x] == '#';
}