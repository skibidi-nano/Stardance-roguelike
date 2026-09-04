#include <ncurses.h>
#include "inventory.h"
#include "config.h"
#include "item.h"

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
    if (item == HEAL)
    {
        mvprintw(position_y,  position_x, "++++++");
        mvprintw(position_y + 1,  position_x, "+ <3 + <<<");
        mvprintw(position_y + 2,  position_x, "++++++");
    }
    else
    {
        mvprintw(position_y,  position_x, "++++++");
        mvprintw(position_y + 1,  position_x, "+    + <<<");
        mvprintw(position_y + 2,  position_x, "++++++");
    }
}

void print_frame_not_select(items item, int position_y, int position_x)
{
    if (item == HEAL)
    {
        mvprintw(position_y,  position_x, "++++++");
        mvprintw(position_y + 1,  position_x, "+ <3 +");
        mvprintw(position_y + 2,  position_x, "++++++");
    }
    else
    {
        mvprintw(position_y,  position_x , "++++++");
        mvprintw(position_y + 1,  position_x + 1, "+    +");
        mvprintw(position_y + 2,  position_x, "++++++");
    }
}
