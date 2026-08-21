#include <ncurses.h>
#include "map.h"
#include "generation.h"
#include "npc.h"

static char map[MAX_HEIGHT][MAX_WIDTH];
static room current_room;


//initialisation of the map array
void map_init(void)
{

    current_room = random_room_gen();

    npc enemy  = npc_init(current_room.width, current_room.height);

    for (int y = 0; y < current_room.height; y++)
    {
        for (int x = 0; x < current_room.width; x++)
        {
            if (y == current_room.door_y && x == current_room.door_x)
            {
                map[y][x] = '0'; //door symbol
            }
            else if (y == enemy.npc_y && x == enemy.npc_x)
            {
                map[y][x] = '&'; //door symbol
            }
            else if (y == 0 || y == current_room.height -1 || x == 0 || x == current_room.width -1)
            {
                map[y][x] = '#'; //wall symbol
            }
            else
            {
                map[y][x] = '.'; //floor symbol
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

int map_is_door(int y, int x)
{
    if (y < 0 || y >= current_room.height || x < 0 || x >= current_room.width)
    {
        return 0;
    }
    return map[y][x] == '0';
}

int map_is_enemy(int y, int x)
{
    if (y < 0 || y >= current_room.height || x < 0 || x >= current_room.width)
    {
        return 0;
    }
    return map[y][x] == '&';
}