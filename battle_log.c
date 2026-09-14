#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include "battle_log.h"
#include "battle_screen.h"
#include "config.h"

void battle_log(actions action, int damage, char log_arr[BATTLE_LOG_HEIGHT][BATTLE_LOG_WIDTH])
{
    static actions previous_action = NONE;
    static actions current_action = NONE;
    static int previous_damage = 0;
    static int current_damage = 0;

    previous_damage = current_damage;
    current_damage = damage;
    previous_action = current_action;
    current_action = action;

    static char msg[BATTLE_LOG_WIDTH];
    static char *str = NULL;

    battle_log_init();

    switch (current_action)
    {
        case PLAYER_ATTACK:
            str = "YOU ATTACKED THE ENEMY";
            snprintf(msg, sizeof(msg), str);
            break;

        case ENEMY_ATTACK:
            str = "ENEMY ATTACKED YOU";
            snprintf(msg, sizeof(msg), str);
            break;


        case PLAYER_DAMAGE_TOOK:
            str = "YOU TOOK %i DAMAGE";
            snprintf(msg, sizeof(msg), str, current_damage);
            break;

        case ENEMY_DAMAGE_TOOK:
            str = "ENEMY TOOK %i DAMAGE";
            snprintf(msg, sizeof(msg), str, current_damage);
            break;
    

        case PLAYER_FAILED_RUN:
            str = "RUNNING FAILED";
            snprintf(msg, sizeof(msg), str);
            break;


        case PLAYER_HEAL_USE:
            str = "YOU USED A HEALING POTION";
            snprintf(msg, sizeof(msg), str);
            break;

        case PLAYER_DAMAGE_USE:
            str = "YOU USED A DAMAGE POTION";
            snprintf(msg, sizeof(msg), str);
            break;

        case PLAYER_POISON_USE:
            str = "YOU USED A POISON POTION";
            snprintf(msg, sizeof(msg), str);
            break;


        case POISON_POTION_TICK:
            str = "POISON DID %i DAMAGE TO YOUR OPPONENT";
            snprintf(msg, sizeof(msg), str, current_damage); //%i turns remaining
            break;

        default:
            break;
    }

    fill_log_arr(2, 2, msg, log_arr);

    switch (previous_action)
    {
        case PLAYER_ATTACK:
            str = "YOU ATTACKED THE ENEMY";
            snprintf(msg, sizeof(msg), str);
            break;

        case ENEMY_ATTACK:
            str = "ENEMY ATTACKED YOU";
            snprintf(msg, sizeof(msg), str);
            break;


        case PLAYER_DAMAGE_TOOK:
            str = "YOU TOOK %i DAMAGE";
            snprintf(msg, sizeof(msg), str, previous_damage);
            break;

        case ENEMY_DAMAGE_TOOK:
            str = "ENEMY TOOK %i DAMAGE";
            snprintf(msg, sizeof(msg), str, previous_damage);
            break;
    

        case PLAYER_FAILED_RUN:
            str = "RUNNING FAILED";
            snprintf(msg, sizeof(msg), str);
            break;


        case PLAYER_HEAL_USE:
            str = "YOU USED A HEALING POTION";
            snprintf(msg, sizeof(msg), str);
            break;

        case PLAYER_DAMAGE_USE:
            str = "YOU USED A DAMAGE POTION";
            snprintf(msg, sizeof(msg), str);
            break;

        case PLAYER_POISON_USE:
            str = "YOU USED A POISON POTION";
            snprintf(msg, sizeof(msg), str);
            break;


        case POISON_POTION_TICK:
            str = "POISON DID %i DAMAGE TO YOUR OPPONENT";
            snprintf(msg, sizeof(msg), str, previous_damage); //%i turns remaining
            break;

        default:
            break;
    }

    fill_log_arr(4, 2, msg, log_arr);
}

void fill_log_arr(int y, int x, char *msg, char log_arr[BATTLE_LOG_HEIGHT][BATTLE_LOG_WIDTH])
{
    snprintf(&log_arr[y][x], BATTLE_LOG_WIDTH - x, "%s", msg);
}