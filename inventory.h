#include "item.h"

void init_inventory_screen(void);
void inventory_screen_draw(int selection, items inventory[]);
void print_frame_select(items item, int position_y, int position_x);
void print_frame_not_select(items item, int position_y, int position_x);
pass assign_item_to_inventory(items inventory[INVENTORY_SIZE], items );