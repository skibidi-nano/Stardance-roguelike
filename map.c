#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "generation.h"
#include "npc.h"
#include "item.h"
#include "battle_screen.h"


static char map[MAX_HEIGHT][MAX_WIDTH];
static char visible_map[MAX_HEIGHT][MAX_WIDTH];
static room current_room; //variable with all measures of the room
static npc room_enemies[MAX_NUMBER_OF_NPCS];  //array of all enemies in the room
static int enemy_counter = 0; //to know which npc has to be killed
static item room_items[MAX_NUMBER_OF_ITEMS]; //array of all items in the room
static int room_counter = 0; //for boss trigger
static int boss_counter = 0; //for dynamic stats

//initialisation of the map array
void map_init(mapstate current_mapstate)
{
    
    // for battle and door logic
    enemy_counter = 0;
    //for boss trigger
    room_counter++;
    
    memset(map, ' ', sizeof(map));
    memset(visible_map, '#', sizeof(map));

    //to gen a fresh room every time
    current_room = random_room_gen();

    switch (current_mapstate)
    {
        case STATE_STANDARD:
            
            //dynamic number of enemies
            int enemy_amnt = (current_room.width * current_room.height) / 200; //<- change this to impact the amount of enemies

            //set up the enemies (only on the map)
            for (int i = 0; i < MAX_NUMBER_OF_NPCS; i++)
            {
                if (i < enemy_amnt)
                {
                    //for which enemies get spawned
                    int decide_enemies = get_random_int(1, enemy_amnt);
                    if (decide_enemies > 2)
                    {
                        room_enemies[i]  = npc_init(current_room.width, current_room.height, MODULO, i, true);//enemy symbol (modulo enemy)
                    }
                    else
                    {
                        room_enemies[i]  = npc_init(current_room.width, current_room.height, ADDRESS, i, true);//enemy symbol (evil address)
                    }
                    enemy_counter++;
                }
                else 
                {
                    room_enemies[i].active = false;
                    room_enemies[i].direction = false;
                    room_enemies[i].npc_x  = MAX_WIDTH - 1;
                    room_enemies[i].npc_y = MAX_HEIGHT - 1;
                }
            }

            //dynamic number of items
            int item_amnt = items_per_room(current_room.width, current_room.height);
    
            //set up the items (only on the map)
            for (int i = 0; i < item_amnt; i++)
            {
                room_items[i] = item_init(current_room.width, current_room.height, i, TRUE, room_enemies, enemy_amnt);
            }

            //init the map array
            for (int y = 0; y < current_room.height; y++)
            {
                for (int x = 0; x < current_room.width; x++)
                {
                    //set up walls etc.
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

                    //loop through all enemies
                    for (int i = 0; i < enemy_amnt; i++) 
                    {
                        if (y == room_enemies[i].npc_y && x == room_enemies[i].npc_x && room_enemies[i].active == TRUE)
                        {
                            switch (room_enemies->npc_type)
                            {
                                case MODULO:
                                    map[y][x] = '%';
                                    break;
                                case ADDRESS:
                                    map[y][x] = '&';
                                    break;
                                default:
                                    break;
                            }
                        }
                    }

                    //loop through all items
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

            //fill the map array for no junk values
            for (int y = 0; y < current_room.height; y++)
            {
                for (int x = 0; x < current_room.width; x++)
                {
                    map[y][x] = ' ';
                }
            }

            //everything is static but thats fine for debugging ig
            map[2][2] = '&';
            map[2][3] = '%';
            map[2][4] = '*';
            for (int i = 0; i < 10; i++)
            {
                map[4][i] = '+';
                map[5][i] = '+';
            }
            map[6][1] = '?';
            break;

        case STATE_BOSS:
        
            //counter variables for
                //battle and door logic
            enemy_counter++;
                //dynamic stats logic
            boss_counter++;
                //there obv only one enemy so the array gets only filled at [0]
            room_enemies[0] = boss_init();

            //fill map with nothing for no junk values bugs (full map because dynamic size sometimes is smaller than boss room size)
            memset(map, ' ', sizeof(map));
            
            //init map array for boss fight
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

            //boss and door
            map[BOSS_POSITION][BOSS_POSITION] = '*';
            map[0][6] = '0';
    }
}

void visible_map_init(int player_y, int player_x)
{
    for (int y = -4; y < 4; y++)
    {
        for (int x = -8; x < 8; x++)
        {
            if ((player_y + y) < MAX_HEIGHT && (player_x + x) < MAX_WIDTH && (player_y + y ) >= 0 && (player_x + x >= 0))
            {
                visible_map[player_y + y][player_x + x] = map[player_y + y][player_x + x];
            }
        }
    }


    for (int y = 0; y < current_room.height; y++)
    {
        for (int x = 0; x < current_room.width; x++)
        {
            if (visible_map[y][x] != map[y][x] && visible_map[y][x] != '#')
            {
                visible_map[y][x] = map[y][x];
            }
        }
    }

    
}

//puts the map on the screen
void map_draw(mapstate current_mapstate)
{
    switch (current_mapstate)
    {
        case STATE_BOSS:
            for (int y = 0; y < BOSS_ROOM_DIMENSION; y++)
            {
                for (int x = 0; x < BOSS_ROOM_DIMENSION; x++)
                {
                    mvaddch(y, x, map[y][x]);
                }
            }
            break;

        case STATE_DEBUG:
            for (int y = 0; y < MAX_HEIGHT; y++)
            {
                for (int x = 0; x < MAX_WIDTH; x++)
                {
                    mvaddch(y, x, map[y][x]);
                }
            }
            break;

        default:
            for (int y = 0; y < current_room.height; y++)
            {
                for (int x = 0; x < current_room.width; x++)
                {
                    mvaddch(y, x, visible_map[y][x]); // change to map for easier map debugging
                    if (x == current_room.width && y == current_room.height)
                    {
                        break;
                    }
                }
            }
    }
}
    
//refreshes the game (duh)
void map_refresh(int player_refresh_y, int player_refresh_x, mapstate current_mapstate)
{
    clear();
    map_draw(current_mapstate);
    mvaddch(player_refresh_y, player_refresh_x, '@');
    refresh();
}


//map_is_xxx explain themselves tbh

int map_is_wall(int y, int x)
{
    //everything outside of the map is treated as a wall
    if (y < 0 || y >= current_room.height || x < 0 || x >= current_room.width)
    {
        return 1;
    }
    if (map[y][x] == '0')
    {
        return 1;
    }
    return map[y][x] == '#';
}

int map_is_door(int y, int x, mapstate *current_mapstate)
{
    if (y < 0 || y >= current_room.height || x < 0 || x >= current_room.width || enemy_counter != 0)
    {
        return 0;
    }
    else if (room_counter == BOSS_ROOM_ACTIVATION_COUNT && map[y][x] == '0')
    {
        return BOSS_ROOM_ACTIVATION_COUNT;
    }
    else if (*current_mapstate == STATE_BOSS && room_enemies[0].active == false && map[y][x] == '0')
    {
        map_init(*current_mapstate);
        *current_mapstate = STATE_STANDARD;
    }

    return map[y][x] == '0';

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
        return 0; //change to 1 for semi debug mode (when going through doors that shouldnt be able to get access, you get items), only works when tweaking wall logic
    }
    if (map[y][x] == '+' || map[y][x] == '?')
    {
        return 1;
    }

    return 0;
}

void map_remove_enemy_at(int y, int x) 
{
    map[y][x] = '.'; //remove enemy item from map
    
    //find the npc in the list to mark as inactive
    for (int i = 0; i < MAX_NUMBER_OF_NPCS; i++) 
    {
        if (room_enemies[i].npc_y == y && room_enemies[i].npc_x == x) 
        {
            room_enemies[i].active = false;
            enemy_counter--; //decrement counter for door logic
            break;
        }
    }

    visible_map_init(y, x);
}

void map_remove_item_at(int y, int x) 
{
    map[y][x] = '.'; //remove item from map
    
    //find the item in the list to mark as inactive
    for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++) 
    {
        if (room_items[i].item_y == y && room_items[i].item_x == x) 
        {
            room_items[i].active = FALSE;
            break;
        }
    }
}

void enemy_pursuit(int player_y, int player_x, mapstate current_mapstate)
{
    direction dir;
    entity (*enemy_ptr)[MAX_NUMBER_OF_NPCS] = get_location_of_enemies(); // get enemy array
    
    for (int i = 0; i < MAX_NUMBER_OF_NPCS; i++)
    {   
        if (room_enemies[i].active == true && current_mapstate != STATE_BOSS)
        {
            
            int dy;
            int dx;

            if (room_enemies[i].direction == true) //chasing
            {
                dy = player_y - room_enemies[i].npc_y; //distance to player
                dx = player_x - room_enemies[i].npc_x;
                dir.dir_y = signum(dy); //calculate direction
                dir.dir_x = signum(dx);
            }
            else //fleeing
            {
                dy = room_enemies[i].npc_y - player_y;  //distance to player
                dx = room_enemies[i].npc_x - player_x;
                dir.dir_y = signum(dy); //calculate direction
                dir.dir_x = signum(dx);

                int current_hp = enemy_ptr[room_enemies[i].npc_type][room_enemies[i].number].current_hp;
                int healing_threshhold = enemy_ptr[room_enemies[i].npc_type][room_enemies[i].number].max_hp / 2; 
                //long story short if enemy current_hp is smaller than enemy max_hp / 2
                if (current_hp < healing_threshhold)
                {
                    enemy_ptr[room_enemies[i].npc_type][room_enemies[i].number].current_hp++;
                }
                else
                {
                    room_enemies[i].direction = true;
                }
            }
                    
            

            if (room_enemies[i].direction == true && (dy == 1 || dy ==  0 || dy == -1))
            {
                if (dx == 1 || dx == 0|| dx == -1)
                {
                    continue;
                }
            }

            int illegal_check = 0;
            int next_enemy_y = room_enemies[i].npc_y + dir.dir_y;
            int next_enemy_x = room_enemies[i].npc_x + dir.dir_x;
            switch (map[next_enemy_y][next_enemy_x])
            {
                case '#':
                case '%':
                case '&':
                    illegal_check++;
                    break;
                default:
                    break;
            }

            if(illegal_check)
            {
                break;
            }

            char tmp = value_of_part_of_map(room_enemies[i].npc_y, room_enemies[i].npc_x);
            map[room_enemies[i].npc_y][room_enemies[i].npc_x] = '.';

            room_enemies[i].npc_y += dir.dir_y;
            room_enemies[i].npc_x += dir.dir_x;


            map[room_enemies[i].npc_y][room_enemies[i].npc_x] = tmp;
        }
    }
}

int signum(int input)
{
    if (input > 0)
    {
        return 1;
    }
    else if (input < 0)
    {
        return -1;
    }
    return 0;
}

void boss_item_creation(void)
{
    map[BOSS_POSITION][BOSS_POSITION] = '?';
}

//pretty self explandatory functions
npc* position_of_enemy_array(void)
{
    return room_enemies;
}

int* position_of_room_counter(void)
{
    return &room_counter;
}

int* position_of_boss_counter(void)
{
    return &boss_counter;
}

char value_of_part_of_map(int y, int x)
{
    return map[y][x];
}

int value_of_boss_counter(void)
{
    return boss_counter;
}

