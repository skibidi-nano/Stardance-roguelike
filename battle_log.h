#ifndef BATTLE_LOG_H
#define BATTLE_LOG_H

#include "config.h"

typedef enum
{
    PLAYER_ATTACK,
    ENEMY_ATTACK,

    PLAYER_DAMAGE_TOOK,
    ENEMY_DAMAGE_TOOK,

    PLAYER_FAILED_RUN,

    PLAYER_HEAL_USE,
    PLAYER_POISON_USE,
    PLAYER_DAMAGE_USE,

    POISON_POTION_TICK,

    NONE
}
actions;

void battle_log(actions action, int damage, char log_arr[BATTLE_LOG_HEIGHT][BATTLE_LOG_WIDTH]);
void fill_log_arr(int y, int x, char *msg, char log_arr[BATTLE_LOG_HEIGHT][BATTLE_LOG_WIDTH]);

#endif