#ifndef CONFIG_H
#define CONFIG_H

//for choosing game state
typedef enum 
{
    STATE_MAP,
    STATE_BATTLE,
    STATE_MENU,
    STATE_ITEM,
    STATE_INVENTORY,
    STATE_CONTROLS
} 
gamestate;

//for deciding which mapstate to print
typedef enum
{
    STATE_STANDARD,
    STATE_BOSS,
    STATE_DEBUG
}
mapstate;

typedef struct
{
    int inventory;
    int battle;
}
selection;

typedef struct
{
    int y;
    int x;
    int next_y;
    int next_x;
    int target_enemy_y;
    int target_enemy_x;
    int target_item_y;
    int target_item_x;
}
player_map;

//for menu size (pls dont change)
#define MENU_WIDTH 56
#define MENU_HEIGHT 14

// SCREENS /////////////////////////////////
//For map gen
#define MAX_WIDTH 50
#define MIN_WIDTH 20
#define MAX_HEIGHT 20
#define MIN_HEIGHT 5

//for item screen gen
#define ITEM_SCREEN_WIDTH 80
#define ITEM_SCREEN_HEIGHT 20

//for inventory screen gen
#define INVENTORY_WIDTH 80
#define INVENTORY_HEIGHT 20

//////////////////////////////////////////////


//MAP/////////////////////////////////////

//for item position
#define ITEM_POS_X 30

//for npc creation
#define MAX_NUMBER_OF_NPCS 8

//for item creation
#define MAX_NUMBER_OF_ITEMS 4

///////////////////////////////////////////


//BATTLE /////////////////////////////////

//for battle screen gen
#define BATTLE_SCREEN_WIDTH 80
#define BATTLE_SCREEN_HEIGHT 20

//for battle
#define MAX_HP_LENGTH 78

//for items in battle
#define HEALING_AMOUNT 5
#define DAMAGE_POTION_AMOUNT 4
#define POISON_AMOUNT 3

//for boss fights
#define BOSS_ROOM_DIMENSION 11
#define BOSS_ROOM_ACTIVATION_COUNT 5
#define BOSS_POSITION 5

//for health bar
#define HEALTH_BAR_POSITION_PLAYER 2
#define HEALTH_BAR_POSITION_ENEMY 5

//for battle log
#define BATTLE_LOG_POSITION_X 3 //will be changed
#define BATTLE_LOG_POSITION_Y_CURRENT 22
#define BATTLE_LOG_POSITION_Y_PREVIOUS 24
#define BATTLE_LOG_HEIGHT 7
#define BATTLE_LOG_WIDTH 40

#define NO_DAMAGE_INPUT 0

////////////////////////////////////////////

//MISC//////////////////////////////////////////

//for selection
#define ENTER 10
#define ESC 27

//for highscore
#define SCORE_FOR_DEFEATING_ENEMY 1
#define SCORE_FOR_DEFEATING_BOSS 5

//for items in main loop
#define INVENTORY_SIZE 10
#define MAXIMUM_ITEMS 5
#define EXTRA_STRENGTH_AMOUNT 1
#define EXTRA_HP_AMOUNT 2

/////////////////////////////////////////////////

#endif