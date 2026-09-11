#include <ncurses.h>
#include <stdlib.h>
#include "map.h"
#include "generation.h"
#include "npc.h"
#include "item.h"


static char map[MAX_HEIGHT][MAX_WIDTH];
static room current_room;
static npc room_enemies[MAX_NUMBER_OF_NPCS]; 
static int enemy_counter = 0; //to know which npc has to be killed
static item room_items[MAX_NUMBER_OF_ITEMS];
static int room_counter = 0;

//initialisation of the map array
void map_init(mapstate current_mapstate)
{
    int decide_enemies = 0;
    room_counter++;
    enemy_counter = 0;

    current_room = random_room_gen();

    switch (current_mapstate)
    {
        case STATE_STANDARD:
            
            int enemy_amnt = (current_room.width * current_room.height) / 200; //dynamic number of enemies

            for (int i = 0; i < enemy_amnt; i++)
            {
                room_enemies[i]  = npc_init(current_room.width, current_room.height, i, TRUE);
                enemy_counter++;
            }

            int item_amnt = items_per_room(current_room.width, current_room.height);
    
            for (int i = 0; i < item_amnt; i++)
            {
                room_items[i] = item_init(current_room.width, current_room.height, i, TRUE, room_enemies, enemy_amnt);
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
                            decide_enemies = get_random_int(1, enemy_amnt);
                            if (decide_enemies > 2)
                            {
                                map[y][x] = '%'; //enemie symbol (alt enemy)
                            }
                            else
                            {
                                map[y][x] = '&'; //enemie symbol (evil pointer)
                            }
                        }
                    }
                    for (int i = 0; i < item_amnt; i++)
                    {
                        if (y == room_items[i].item_y && x == room_items[i].item_x && room_items[i].active == TRUE)
                        {
                            map[y][x] = '+'; //item symbol
                        }
                    }
                }
            }
            break;

        case STATE_DEBUG: 

            for (int y = 0; y < current_room.height; y++)
            {
                for (int x = 0; x < current_room.width; x++)
                {
                    map[y][x] = ' ';
                }
            }
            map[2][2] = '&';
            map[2][3] = '%';
            for (int i = 0; i < 10; i++)
            {
                map[4][i] = '+';
            }
            break;

        case STATE_BOSS:
        
            enemy_counter++;
            room_enemies[0] = boss_init();
            for (int y = 0; y < MAX_HEIGHT; y++)
            {
                for (int x = 0; x < MAX_WIDTH; x++)
                {
                    map[y][x] = ' ';
                }
            }
            for (int y = 0; y < BOSS_ROOM_DIMENSION; y++)
            {
                for (int x = 0; x < BOSS_ROOM_DIMENSION; x++)
                {
                    if (y == 0 || y == BOSS_ROOM_DIMENSION -1 || x == 0 || x == BOSS_ROOM_DIMENSION -1)
                    {
                        map[y][x] = '#';
                    }
                    else
                    {
                        map[y][x] = '.';
                    }
                }
            }
            map[5][5] = '*';
            map[0][6] = '0';
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
            if (x == current_room.width && y == current_room.height)
            {
                break;
            }
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
    else if (room_counter == BOSS_ROOM_ACTIVATION_COUNT && map[y][x] == '0')
    {
        return BOSS_ROOM_ACTIVATION_COUNT;
    }
    else
    {
        return map[y][x] == '0';
    }
}

int map_is_enemy(int y, int x)
{
    if (y < 0 || y >= current_room.height || x < 0 || x >= current_room.width)
    {
        return 0;
    }
    else if (map[y][x] == '&' || map[y][x] == '%' || map[y][x] == '*')
    {
        return 1;
    }
    return 0;
}

int map_is_item(int y, int x)
{
    
    if (y < 0 || y >= current_room.height || x < 0 || x >= current_room.width)
    {
        return 1; //change to 1 for semi debug mode (when going through doors that shouldnt be able to get access, you get items)
    }
    return map[y][x] == '+';
}

npc* position_of_enemy_array(void)
{
    return room_enemies;
}

char value_of_part_of_map(int y, int x)
{
    return map[y][x];
}

int* position_of_room_counter(void)
{
    return &room_counter;
}

void map_remove_enemy_at(int y, int x) 
{
    map[y][x] = '.'; // Remove enemy
    
    // Find the NPC in our list to mark as inactive
    for (int i = 0; i < MAX_NUMBER_OF_NPCS; i++) 
    {
        if (room_enemies[i].npc_y == y && room_enemies[i].npc_x == x) 
        {
            room_enemies[i].active = false;
            enemy_counter--; // Decrement counter for door logic
            break;
        }
    }
}

void map_remove_item_at(int y, int x) 
{
    map[y][x] = '.'; // Remove enemy
    
    // Find the NPC in our list to mark as inactive
    for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++) 
    {
        if (room_items[i].item_y == y && room_items[i].item_x == x) 
        {
            room_items[i].active = FALSE;
            break;
        }
    }
}


