#include <ncurses.h>
#include "inventory.h"
#include "config.h"
#include "item.h"
#include "map.h"

//need to make the frame drawing logic a bit better with macros and overall needs to be done a tad bit better

char inventory_screen[INVENTORY_HEIGHT][INVENTORY_WIDTH];

void init_inventory_screen(void)
{
    for (int y = 0; y < INVENTORY_HEIGHT; y++)
    {
        for (int x = 0; x < INVENTORY_WIDTH; x++)
        {
            if (y == 0 || y == INVENTORY_HEIGHT -1 || x == 0 || x == INVENTORY_WIDTH -1)
            {
                inventory_screen[y][x] = '#';
            }
            else
            {
                inventory_screen[y][x] = '.';
            }

        }
    }
}

void inventory_screen_draw(int selection, items inventory[])
{
    for (int y = 0; y < INVENTORY_HEIGHT; y++)
    {
        for (int x = 0; x < INVENTORY_WIDTH; x++)
        {
            mvaddch(y, x, inventory_screen[y][x]);
        }
    }

    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        if (selection == i)
        {
            if(i < 5)
            {
                print_frame_select(inventory[i], 5, (i * 9) + 5);
            }
            else
            {
                print_frame_select(inventory[i], 10, (i * 9) - 41);
            }
        }
        else
        {
            if(i < 5)
            {
                print_frame_not_select(inventory[i], 5, (i * 9) + 5);
            }
            else
            {
                print_frame_not_select(inventory[i], 10, (i * 9) - 41);
            }
        }
    }
}

void print_frame_select(items item, int position_y, int position_x)
{
    switch (item)
    {
        case HEAL:
            mvprintw(position_y,  position_x, "++++++");
            mvprintw(position_y + 1,  position_x, "+ <3 + <<<");
            mvprintw(position_y + 2,  position_x, "++++++");
            mvprintw(3, 5, "HEALS YOU FOR %i HP", HEALING_AMOUNT);
            break;

        case DAMAGE:
            mvprintw(position_y,  position_x, "++++++");
            mvprintw(position_y + 1,  position_x, "+ !  + <<<");
            mvprintw(position_y + 2,  position_x, "++++++");
            mvprintw(3, 5, "DEALS %i HP OF DAMAGE TO YOUR OPPONENT", DAMAGE_POTION_AMOUNT);
            break;

        case POISON:
            mvprintw(position_y,  position_x, "++++++");
            mvprintw(position_y + 1,  position_x, "+X_X + <<<");
            mvprintw(position_y + 2,  position_x, "++++++");
            mvprintw(3, 5, "DEALS %i HP OF DAMAGE TO YOUR OPPONENT FOR 3 ROUNDS", POISON_AMOUNT);
            break;

        case BOSS_ITEM:
            mvprintw(position_y,  position_x, "++++++");
            mvprintw(position_y + 1,  position_x, "+****+ <<<");
            mvprintw(position_y + 2,  position_x, "++++++");
            mvprintw(3, 5, "MAKES YOU IMMUNE TO DAMAGE FOR TWO TURNS");
            break;

        case EMPTY:
            mvprintw(position_y,  position_x, "++++++");
            mvprintw(position_y + 1,  position_x, "+    + <<<");
            mvprintw(position_y + 2,  position_x, "++++++");
            break;

        default:
            break;
    }
}

void print_frame_not_select(items item, int position_y, int position_x)
{
    switch (item)
    {
        case HEAL:
            mvprintw(position_y,  position_x, "++++++");
            mvprintw(position_y + 1,  position_x, "+ <3 +");
            mvprintw(position_y + 2,  position_x, "++++++");
            break;

        case DAMAGE:
            mvprintw(position_y,  position_x, "++++++");
            mvprintw(position_y + 1,  position_x, "+ !  +");
            mvprintw(position_y + 2,  position_x, "++++++");

            break;

        case POISON:
            mvprintw(position_y,  position_x, "++++++");
            mvprintw(position_y + 1,  position_x, "+X_X +");
            mvprintw(position_y + 2,  position_x, "++++++");

            break;

        case BOSS_ITEM:
            mvprintw(position_y,  position_x, "++++++");
            mvprintw(position_y + 1,  position_x, "+****+");
            mvprintw(position_y + 2,  position_x, "++++++");
            break;

        case EMPTY:
            mvprintw(position_y,  position_x, "++++++");
            mvprintw(position_y + 1,  position_x, "+    +");
            mvprintw(position_y + 2,  position_x, "++++++");
            break;

        default:
            break;
    }
}

pass assign_item_to_inventory(items inventory[INVENTORY_SIZE], items current_item)
{
    int item_check = 0;
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        if (inventory[i] == EMPTY)
        {
            inventory[i] = current_item;
            item_check++;
            return ENTER_LEGAL;
        }
    }

    if (item_check  == 0 && current_item == BOSS_ITEM)
    {
        inventory[0] = current_item;
        return ENTER_LEGAL;
    }

    return ENTER_ILLEGAL;
}
