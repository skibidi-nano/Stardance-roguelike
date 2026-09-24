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

    battle_log_init();

    switch (current_action)
    {
        case PLAYER_ATTACK:
            snprintf(msg, sizeof(msg), "YOU ATTACKED THE ENEMY");
            break;

        case ENEMY_ATTACK:
            snprintf(msg, sizeof(msg), "ENEMY ATTACKED YOU");
            break;


        case PLAYER_DAMAGE_TOOK:
            snprintf(msg, sizeof(msg), "YOU TOOK %i DAMAGE", current_damage);
            break;

        case ENEMY_DAMAGE_TOOK:
            snprintf(msg, sizeof(msg), "ENEMY TOOK %i DAMAGE", current_damage);
            break;
    

        case PLAYER_FAILED_RUN:
            snprintf(msg, sizeof(msg), "RUNNING FAILED");
            break;


        case PLAYER_HEAL_USE:
            snprintf(msg, sizeof(msg), "YOU USED A HEALING POTION");
            break;

        case PLAYER_DAMAGE_USE:
            snprintf(msg, sizeof(msg), "YOU USED A DAMAGE POTION");
            break;

        case PLAYER_POISON_USE:
            snprintf(msg, sizeof(msg), "YOU USED A POISON POTION");
            break;

        case PLAYER_BOSS_ITEM_USE:
            snprintf(msg, sizeof(msg), "YOU USED A BOSS POTION");
            break;

        case POISON_POTION_TICK:
            snprintf(msg, sizeof(msg), "POISON DID %i DAMAGE TO YOUR OPPONENT", current_damage); //%i turns remaining
            break;

        default:
            break;
    }

    fill_log_arr(2, 2, msg, log_arr);

    switch (previous_action)
    {
        case PLAYER_ATTACK:
            snprintf(msg, sizeof(msg), "YOU ATTACKED THE ENEMY");
            break;

        case ENEMY_ATTACK:
            snprintf(msg, sizeof(msg), "ENEMY ATTACKED YOU");
            break;


        case PLAYER_DAMAGE_TOOK:
            snprintf(msg, sizeof(msg), "YOU TOOK %i DAMAGE", previous_damage);
            break;

        case ENEMY_DAMAGE_TOOK:
            snprintf(msg, sizeof(msg), "ENEMY TOOK %i DAMAGE", previous_damage);
            break;
    

        case PLAYER_FAILED_RUN:
            snprintf(msg, sizeof(msg), "RUNNING FAILED");
            break;


        case PLAYER_HEAL_USE:
            snprintf(msg, sizeof(msg), "YOU USED A HEALING POTION");
            break;

        case PLAYER_DAMAGE_USE:
            snprintf(msg, sizeof(msg), "YOU USED A DAMAGE POTION");
            break;

        case PLAYER_POISON_USE:
            snprintf(msg, sizeof(msg), "YOU USED A POISON POTION");
            break;

        case PLAYER_BOSS_ITEM_USE:
            snprintf(msg, sizeof(msg), "YOU USED A BOSS POTION");
            break;


        case POISON_POTION_TICK:
            snprintf(msg, sizeof(msg), "POISON DID %i DAMAGE TO YOUR OPPONENT", previous_damage); //%i turns remaining
            break;

        default:
            break;
    }

    fill_log_arr(4, 2, msg, log_arr);
}

void fill_log_arr(int y, int x, char *msg, char log_arr[BATTLE_LOG_HEIGHT][BATTLE_LOG_WIDTH])
{
    snprintf(&log_arr[y][x], BATTLE_LOG_WIDTH, "%s", msg);
}