#include <ncurses.h>
#include "map.h"
#include "generation.h"
#include "npc.h"

static char map[MAX_HEIGHT][MAX_WIDTH];
static room current_room;
static npc room_enemies[MAX_NUMBER_OF_NPCS]; 
static int enemy_counter = 0; //to know which npc has to be killed


//initialisation of the map array
void map_init(void)
{
    enemy_counter = 0;

    current_room = random_room_gen();

    int enemy_amnt = (current_room.width * current_room.height) / 200; //dynamic number of enemies

    for (int i = 0; i < enemy_amnt; i++)
    {
        room_enemies[i]  = npc_init(current_room.width, current_room.height, i, TRUE);
        enemy_counter++;
    }
    

    for (int y = 0; y < current_room.height; y++)
    {
        for (int x = 0; x < current_room.width; x++)
        {
            if (y == current_room.door_y && x == current_room.door_x)
            {
                map[y][x] = '0'; //door symbol
            }
            else if (y == 0 || y == current_room.height -1 || x == 0 || x == current_room.width -1)
            {
                map[y][x] = '#'; //wall symbol
            }
            else
            {
                map[y][x] = '.'; //floor symbol
            }
            for (int i = 0; i < enemy_amnt; i++) 
            {
                if (y == room_enemies[i].npc_y && x == room_enemies[i].npc_x && room_enemies[i].active == TRUE)
                {
                    map[y][x] = '&'; //enemie symbol (evil pointer)
                }
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
    if (y < 0 || y >= current_room.height || x < 0 || x >= current_room.width || enemy_counter != 0)
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

npc* position_of_enemy_array(void)
{
    return room_enemies;
}

void map_remove_enemy_at(int y, int x) 
{
    map[y][x] = '.'; // Remove enemy
    
    // Find the NPC in our list to mark as inactive
    for (int i = 0; i < MAX_NUMBER_OF_NPCS; i++) 
    {
        if (room_enemies[i].npc_y == y && room_enemies[i].npc_x == x) 
        {
            room_enemies[i].active = FALSE;
            enemy_counter--; // Decrement counter for door logic
            break;
        }
    }
}