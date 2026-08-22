#ifndef CONFIG_H
#define CONFIG_H

//for choosing game state
typedef enum 
{
    STATE_MAP,
    STATE_BATTLE,
    STATE_MENU //coming soon!!!
} gamestate;

//For menu size (pls dont change)
#define MENU_WIDTH 50
#define MENU_HEIGHT 10

//For map gen
#define MAX_WIDTH 50
#define MIN_WIDTH 20
#define MAX_HEIGHT 20
#define MIN_HEIGHT 5

//For battle screen gen
#define BATTLE_SCREEN_WIDTH 80
#define BATTLE_SCREEN_HEIGHT 40

#endif