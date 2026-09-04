#include <ncurses.h>
#include <string.h>
#include "item.h"
#include "config.h"
#include "npc.h"
#include "generation.h"

static char item_screen[ITEM_SCREEN_HEIGHT][ITEM_SCREEN_WIDTH];

item item_init(int map_width, int map_height, int number, bool state, npc enemy[], int enemy_amnt)
{
    item floor_item;
    bool valid_position;

    // collision check for starting position and enemy position
    do {
        valid_position = true;
        floor_item.item_x = get_random_int(1, map_width - 2);
        floor_item.item_y = get_random_int(1, map_height - 2);

        // block the starting position
        if (floor_item.item_x == 1 && floor_item.item_y == 1) 
        {
            valid_position = false;
            continue;
        }

        // check if item position is the same as enemy position
        for (int i = 0; i < enemy_amnt; i++) 
        {
            if (floor_item.item_x == enemy[i].npc_x && floor_item.item_y == enemy[i].npc_y) {
                valid_position = false;
                break;
            }
        }
    } while (!valid_position);

    floor_item.number = number;
    floor_item.active = state;

    return floor_item;
}

int items_per_room(int room_width, int room_height)
{
    int room_size = room_height * room_width;

    if (room_size > 599) return get_random_int(0, 3);
    if (room_size > 300) return get_random_int(0, 2);
    if (room_size > 100) return get_random_int(0, 1);

    return 0;
}

items random_item(void)
{
    static const items possible_items[] = 
    {
        HEAL, HEAL, HEAL, HEAL, HEAL, HEAL, 
        EXTRA_STRENGTH, EXTRA_STRENGTH, 
        EXTRA_HP, EXTRA_HP
    };
    
    int count = sizeof(possible_items) / sizeof(possible_items[0]);
    int x = get_random_int(0, count - 1);
    
    return possible_items[x];
}

void init_item_screen(void)
{
    memset(item_screen, ' ', sizeof(item_screen));
}

void item_screen_draw(items item)
{
    for (int y = 0; y < ITEM_SCREEN_HEIGHT; y++)
    {
        mvaddnstr(y, 0, item_screen[y], ITEM_SCREEN_WIDTH);
    }

    switch (item)
    {
        case HEAL:
            heal_sprite();
            mvaddnstr(2, 31, "YOU'VE GOT A HEALING POTION", -1);
            break;
        case EXTRA_STRENGTH:
            extra_strength_sprite();
            mvaddnstr(2, 31, "YOU'VE GOT A STRENGTH GAUNTLET", -1);
            break;
        case EXTRA_HP:
            extra_hp_sprite();
            mvaddnstr(2, 31, "YOU'VE GOT AN EXTRA HP ENCHANTMENT", -1);
            break;
        default:
            break;
    }

    mvaddnstr(18, 31, "PRESS \"ESCAPE\" TO RETURN", -1);

    mvaddnstr(19, 31, "PRESS \"ENTER\" TO PICK UP", -1);
}

void heal_sprite(void)
{
    mvprintw(2,  ITEM_POS_Y, "       ( (       ");
    mvprintw(3,  ITEM_POS_Y, "        ) )      ");
    mvprintw(4,  ITEM_POS_Y, "      .----+----.");
    mvprintw(5,  ITEM_POS_Y, "      |  _---_  |");
    mvprintw(6,  ITEM_POS_Y, "      | /     \\ |");
    mvprintw(7,  ITEM_POS_Y, "      | |  |  | |");
    mvprintw(8,  ITEM_POS_Y, "      | | -+- | |");
    mvprintw(9,  ITEM_POS_Y, "      | |  |  | |");
    mvprintw(10, ITEM_POS_Y, "      | \\_   _/ |");
    mvprintw(11, ITEM_POS_Y, "      |   \"\"    |");
    mvprintw(12, ITEM_POS_Y, "     /           \\");
    mvprintw(13, ITEM_POS_Y, "    |  .-------.  |");
    mvprintw(14, ITEM_POS_Y, "    |  |~~~~~~~|  |");
    mvprintw(15, ITEM_POS_Y, "    |  |~~~~~~~|  |");
    mvprintw(16, ITEM_POS_Y, "     \\ '-------' /");
    mvprintw(17, ITEM_POS_Y, "      `---------'");
}

void extra_hp_sprite(void)
{
    mvprintw(2,  ITEM_POS_Y, "       .-\"\"-.      .-\"\"-.       ");
    mvprintw(3,  ITEM_POS_Y, "     /        \\  /        \\      ");
    mvprintw(4,  ITEM_POS_Y, "    |          \\/          |     ");
    mvprintw(5,  ITEM_POS_Y, "    |   .--------------.   |     ");
    mvprintw(6,  ITEM_POS_Y, "     \\  |    /\\  /\\    |  /      ");
    mvprintw(7,  ITEM_POS_Y, "      \\ |   /  \\/  \\   | /       ");
    mvprintw(8,  ITEM_POS_Y, "       \\|  /        \\  |/        ");
    mvprintw(9,  ITEM_POS_Y, "        |  \\        /  |         ");
    mvprintw(10, ITEM_POS_Y, "         \\  \\      /  /          ");
    mvprintw(11, ITEM_POS_Y, "          \\  \\    /  /           ");
    mvprintw(12, ITEM_POS_Y, "           \\  '--'  /            ");
    mvprintw(13, ITEM_POS_Y, "            \\      /             ");
    mvprintw(14, ITEM_POS_Y, "             `----'              ");  
}

void extra_strength_sprite(void)
{
    mvprintw(2,  ITEM_POS_Y, "       .--------------.       ");
    mvprintw(3,  ITEM_POS_Y, "      /   ______      \\      ");
    mvprintw(4,  ITEM_POS_Y, "     /   /      \\      \\     ");
    mvprintw(5,  ITEM_POS_Y, "    |   |  .---. |      |    ");
    mvprintw(6,  ITEM_POS_Y, "    |   |  |   | |  |||||    ");
    mvprintw(7,  ITEM_POS_Y, "    |   |  | + | |  |||||    ");
    mvprintw(8,  ITEM_POS_Y, "    |   |  '---' |  |''''    ");
    mvprintw(9,  ITEM_POS_Y, "    |   \\______/    |        ");
    mvprintw(10, ITEM_POS_Y, "    |  .----------. |        ");
    mvprintw(11, ITEM_POS_Y, "    |  |  ______  | |        ");
    mvprintw(12, ITEM_POS_Y, "     \\ | |      | |/         ");
    mvprintw(13, ITEM_POS_Y, "      \\| |______| |          ");
    mvprintw(14, ITEM_POS_Y, "       '----------'          ");
}