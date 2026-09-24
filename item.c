#include <ncurses.h>
#include <string.h>
#include "item.h"
#include "config.h"
#include "npc.h"
#include "generation.h"
#include "battle_screen.h"

static char item_screen[ITEM_SCREEN_HEIGHT][ITEM_SCREEN_WIDTH];

item item_init(int map_width, int map_height, int number, bool state, npc enemy[], int enemy_amnt)
{
    item floor_item;
    bool valid_position;

    //collision check for starting position and enemy position
    do {
        valid_position = true;
        floor_item.item_x = get_random_int(1, map_width - 2);
        floor_item.item_y = get_random_int(1, map_height - 2);

        //block the starting position
        if (floor_item.item_x == 1 && floor_item.item_y == 1) 
        {
            valid_position = false;
            continue;
        }

        //check if item position is the same as enemy position
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
    // array of all possible items
    static const items possible_items[] = 
    {
        HEAL, HEAL, HEAL, HEAL, HEAL, HEAL, 
        EXTRA_STRENGTH, EXTRA_STRENGTH, 
        EXTRA_HP, EXTRA_HP, DAMAGE, DAMAGE, POISON
    };
    
    int count = sizeof(possible_items) / sizeof(possible_items[0]);
    int x = get_random_int(0, count - 1);
    
    return possible_items[x];
}

void init_item_screen(void)
{
    //easy way to fill the entire screen with blank values
    memset(item_screen, ' ', sizeof(item_screen));
}

void item_screen_draw(items item, bool inventory_full)
{
    for (int y = 0; y < ITEM_SCREEN_HEIGHT; y++)
    {
        mvaddnstr(y, 0, item_screen[y], ITEM_SCREEN_WIDTH);
    }

    switch (item)
    {
        case HEAL:
            heal_sprite();
            mvaddnstr(1, 31, "YOU'VE GOT A HEALING POTION", -1);
            break;

        case DAMAGE:
            damage_sprite();
            mvaddnstr(1, 31, "YOU'VE GOT A DAMAGE POTION", -1);
            break;

        case POISON:
            poison_sprite();
            mvaddnstr(1, 31, "YOU'VE GOT A POISON POTION", -1);
            break;

        case BOSS_ITEM:
            boss_item_sprite();
            mvaddnstr(1, 31, "YOU'VE GOT THE BOSS POTION", -1);
            break;

        case EXTRA_STRENGTH:
            extra_strength_sprite();
            mvaddnstr(1, 31, "YOU'VE GOT A STRENGTH GAUNTLET", -1);
            break;

        case EXTRA_HP:
            extra_hp_sprite();
            mvaddnstr(1, 31, "YOU'VE GOT AN EXTRA HP ENCHANTMENT", -1);
            break;

        default:
            break;
    }

    if (inventory_full)
    {
        mvaddnstr(9, ITEM_POS_X, "!! INVENTORY FULL, PRESS ESC !!", -1);
    }

    mvaddnstr(19, 31, "PRESS \"ESCAPE\" TO RETURN", -1);

    mvaddnstr(20, 31, "PRESS \"ENTER\" TO PICK UP", -1);
}

void heal_potion(void)
{
    int *heal_manipulator = NULL;
    int *max_hp_ptr = NULL;
    max_hp_ptr = get_location_of(EXTRA_HP);
    heal_manipulator = get_location_of(HEAL);
    if (*heal_manipulator + HEALING_AMOUNT > *max_hp_ptr)
    {
        *heal_manipulator = *max_hp_ptr;
    }
    else
    {
        *heal_manipulator += HEALING_AMOUNT;
    }
}

void damage_potion(void)
{
    int *enemy_life_manipulator = NULL;
    enemy_life_manipulator = get_location_of(DAMAGE);
    if (*enemy_life_manipulator - DAMAGE_POTION_AMOUNT < 0)
    {
        *enemy_life_manipulator = 0;
    }
    else
    {
        *enemy_life_manipulator -= DAMAGE_POTION_AMOUNT;
    }
}

void poison_potion(void)
{
    int *enemy_life_manipulator = NULL;
    enemy_life_manipulator = get_location_of(DAMAGE);
    if (*enemy_life_manipulator - POISON_AMOUNT < 0)
    {
        *enemy_life_manipulator = 0;
    }
    else
    {
        *enemy_life_manipulator -= POISON_AMOUNT;
    }

}

void poison_enabler(bool input)
{
    bool* poison_manipulator = NULL;
    poison_manipulator = get_location_of_poison();
    *poison_manipulator = input;
}

void poison_call(int turn_counter)
{   
    static bool first_call = true;
    static int initial_turn;
    
    if(first_call)
    {
        initial_turn = turn_counter;
        first_call = false;
    }

    poison_potion();
    if (initial_turn == turn_counter - 2)
    {
        poison_enabler(false);
        first_call = true;
    }
}



void heal_sprite(void)
{
    mvprintw(2,  ITEM_POS_X, "       ( (       ");
    mvprintw(3,  ITEM_POS_X, "        ) )      ");
    mvprintw(4,  ITEM_POS_X, "      .----+----.");
    mvprintw(5,  ITEM_POS_X, "      |  _---_  |");
    mvprintw(6,  ITEM_POS_X, "      | /     \\ |");
    mvprintw(7,  ITEM_POS_X, "      | |  |  | |");
    mvprintw(8,  ITEM_POS_X, "      | | -+- | |");
    mvprintw(9,  ITEM_POS_X, "      | |  |  | |");
    mvprintw(10, ITEM_POS_X, "      | \\_   _/ |");
    mvprintw(11, ITEM_POS_X, "      |   \"\"    |");
    mvprintw(12, ITEM_POS_X, "     /           \\");
    mvprintw(13, ITEM_POS_X, "    |  .-------.  |");
    mvprintw(14, ITEM_POS_X, "    |  |~~~~~~~|  |");
    mvprintw(15, ITEM_POS_X, "    |  |~~~~~~~|  |");
    mvprintw(16, ITEM_POS_X, "     \\ '-------' /");
    mvprintw(17, ITEM_POS_X, "      `---------'");
}

void damage_sprite(void)
{
    mvprintw(2,  ITEM_POS_X, "       ( (       ");
    mvprintw(3,  ITEM_POS_X, "        ) )      ");
    mvprintw(4,  ITEM_POS_X, "      .----+----.");
    mvprintw(5,  ITEM_POS_X, "      |  _---_  |");
    mvprintw(6,  ITEM_POS_X, "      | /     \\ |");
    mvprintw(7,  ITEM_POS_X, "      | | (o) | |");
    mvprintw(8,  ITEM_POS_X, "      | | / \\ | |");
    mvprintw(9,  ITEM_POS_X, "      | |  ~  | |");
    mvprintw(10, ITEM_POS_X, "      | \\_   _/ |");
    mvprintw(11, ITEM_POS_X, "      |   \"\"    |");
    mvprintw(12, ITEM_POS_X, "     /           \\");
    mvprintw(13, ITEM_POS_X, "    |  .-------.  |");
    mvprintw(14, ITEM_POS_X, "    |  |x x x x|  |");
    mvprintw(15, ITEM_POS_X, "    |  |x x x x|  |");
    mvprintw(16, ITEM_POS_X, "     \\ '-------' /");
    mvprintw(17, ITEM_POS_X, "      `---------'");
}

void poison_sprite(void)
{
    mvprintw(2,  ITEM_POS_X, "       ( (       ");
    mvprintw(3,  ITEM_POS_X, "        ) )      ");
    mvprintw(4,  ITEM_POS_X, "      .----+----.");
    mvprintw(5,  ITEM_POS_X, "      |  _---_  |");
    mvprintw(6,  ITEM_POS_X, "      | /     \\ |");
    mvprintw(7,  ITEM_POS_X, "      | |  _  | |");
    mvprintw(8,  ITEM_POS_X, "      | | (o) | |");
    mvprintw(9,  ITEM_POS_X, "      | | /|\\ | |");
    mvprintw(10, ITEM_POS_X, "      | \\_   _/ |");
    mvprintw(11, ITEM_POS_X, "      |   \"\"    |");
    mvprintw(12, ITEM_POS_X, "     /           \\");
    mvprintw(13, ITEM_POS_X, "    |  .-------.  |");
    mvprintw(14, ITEM_POS_X, "    |  |o O o O|  |");
    mvprintw(15, ITEM_POS_X, "    |  | O o O o| |");
    mvprintw(16, ITEM_POS_X, "     \\ '-------' /");
    mvprintw(17, ITEM_POS_X, "      `---------'");
}

void extra_hp_sprite(void)
{
    mvprintw(2,  ITEM_POS_X, "       .-\"\"-.      .-\"\"-.       ");
    mvprintw(3,  ITEM_POS_X, "     /        \\  /        \\      ");
    mvprintw(4,  ITEM_POS_X, "    |          \\/          |     ");
    mvprintw(5,  ITEM_POS_X, "    |   .--------------.   |     ");
    mvprintw(6,  ITEM_POS_X, "     \\  |    /\\  /\\    |  /      ");
    mvprintw(7,  ITEM_POS_X, "      \\ |   /  \\/  \\   | /       ");
    mvprintw(8,  ITEM_POS_X, "       \\|  /        \\  |/        ");
    mvprintw(9,  ITEM_POS_X, "        |  \\        /  |         ");
    mvprintw(10, ITEM_POS_X, "         \\  \\      /  /          ");
    mvprintw(11, ITEM_POS_X, "          \\  \\    /  /           ");
    mvprintw(12, ITEM_POS_X, "           \\  '--'  /            ");
    mvprintw(13, ITEM_POS_X, "            \\      /             ");
    mvprintw(14, ITEM_POS_X, "             `----'              ");  
}

void extra_strength_sprite(void)
{
    mvprintw(2,  ITEM_POS_X, "       .--------------.       ");
    mvprintw(3,  ITEM_POS_X, "      /   ______      \\      ");
    mvprintw(4,  ITEM_POS_X, "     /   /      \\      \\     ");
    mvprintw(5,  ITEM_POS_X, "    |   |  .---. |      |    ");
    mvprintw(6,  ITEM_POS_X, "    |   |  |   | |  |||||    ");
    mvprintw(7,  ITEM_POS_X, "    |   |  | + | |  |||||    ");
    mvprintw(8,  ITEM_POS_X, "    |   |  '---' |  |''''    ");
    mvprintw(9,  ITEM_POS_X, "    |   \\______/    |        ");
    mvprintw(10, ITEM_POS_X, "    |  .----------. |        ");
    mvprintw(11, ITEM_POS_X, "    |  |  ______  | |        ");
    mvprintw(12, ITEM_POS_X, "     \\ | |      | |/         ");
    mvprintw(13, ITEM_POS_X, "      \\| |______| |          ");
    mvprintw(14, ITEM_POS_X, "       '----------'          ");
}

void boss_item_sprite(void)
{
    mvprintw(2,  ITEM_POS_X, "       ( (       ");
    mvprintw(3,  ITEM_POS_X, "        ) )      ");
    mvprintw(4,  ITEM_POS_X, "      .----+----.");
    mvprintw(5,  ITEM_POS_X, "      |  _---_  |");
    mvprintw(6,  ITEM_POS_X, "      | /     \\ |");
    mvprintw(7,  ITEM_POS_X, "      | |  *  | |");
    mvprintw(8,  ITEM_POS_X, "      | | / \\ | |");
    mvprintw(9,  ITEM_POS_X, "      | | \\_/ | |");
    mvprintw(10, ITEM_POS_X, "      | \\_   _/ |");
    mvprintw(11, ITEM_POS_X, "      |   \"\"    |");
    mvprintw(12, ITEM_POS_X, "     /           \\");
    mvprintw(13, ITEM_POS_X, "    |  .-------.  |");
    mvprintw(14, ITEM_POS_X, "    |  |* * * *|  |");
    mvprintw(15, ITEM_POS_X, "    |  | * * * |  |");
    mvprintw(16, ITEM_POS_X, "     \\ '-------' /");
    mvprintw(17, ITEM_POS_X, "      `---------'");

}