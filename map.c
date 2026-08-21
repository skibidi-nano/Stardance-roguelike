#include <ncurses.h>
#include "map.h"
#include "generation.h"

static char map[MAX_HEIGHT][MAX_WIDTH];
static room current_room;


//initialisation of the map array
void map_init(void)
{

    current_room = random_room_gen();
    for (int y = 0; y < current_room.height; y++)
    {
        for (int x = 0; x < current_room.width; x++)
        {
            if (y == 0 || y == current_room.height -1 || x == 0 || x == current_room.width -1)
            {
                map[y][x] = '#';
            }
            else
            {
                map[y][x] = '.';
            }
        }
    }
}

//Puts the map on the screen
void map_draw(void)
{
    for (int y = 0; y < current_room.height; y++)
    {
        for (int x = 0; x < current_room.width; x++)
        {
            mvaddch(y, x, map[y][x]);
        }
    }
}

//checks for walls
int map_is_wall(int y, int x)
{
    //everything outside of the map is treated as a wall
    if (y < 0 || y >= current_room.height || x < 0 || x >= current_room.width)
    {
        return 1;
    }
    return map[y][x] == '#';
}