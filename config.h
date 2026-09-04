#ifndef CONFIG_H
#define CONFIG_H

//for choosing game state
typedef enum 
{
    STATE_MAP,
    STATE_BATTLE,
    STATE_MENU,
    STATE_ITEM,
    STATE_INVENTORY
} gamestate;


//For menu size (pls dont change)
#define MENU_WIDTH 56
#define MENU_HEIGHT 10

//For map gen
#define MAX_WIDTH 50
#define MIN_WIDTH 20
#define MAX_HEIGHT 20
#define MIN_HEIGHT 5

//For battle screen gen
#define BATTLE_SCREEN_WIDTH 80
#define BATTLE_SCREEN_HEIGHT 20

//For item screen gen
#define ITEM_SCREEN_WIDTH 80
#define ITEM_SCREEN_HEIGHT 20

//For invenoty screen gen
#define INVENTORY_WIDTH 80
#define INVENTORY_HEIGHT 20

//For npc creation
#define MAX_NUMBER_OF_NPCS 8

//For item creation
#define MAX_NUMBER_OF_ITEMS 4

//For battle selection
#define ENTER 10
#define ESC 27

//for highscore
#define SCORE_FOR_DEFEATING_ENEMY 1

//for item position
#define ITEM_POS_Y 30

//for items in main loop
#define INVENTORY_SIZE 10
#define MAXIMUM_ITEMS 5
#define EXTRA_STRENGTH_AMOUNT 5
#define EXTRA_HP_AMOUNT 3
#define HEALING_AMOUNT 5

//for health bar
#define HEALTH_BAR_POSITION_PLAYER 2
#define HEALTH_BAR_POSITION_ENEMY 4

#endif