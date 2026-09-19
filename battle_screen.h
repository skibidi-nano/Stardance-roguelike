#include "item.h"
#include "battle_log.h"

typedef enum
{
    TURN_PLAYER,
    TURN_ENEMY,
} turn_player;

typedef enum
{
    BATTLE_IN_PROGRESS,
    BATTLE_VICTORY,
    BATTLE_DEFEAT,
    BATTLE_FLED,
    BATTLE_SETUP,
    BATTLE_INVENTORY,
    BATTLE_ERROR
} battle_result;

typedef enum
{
    ATTACK,
    INVENTORY,
    RUN
} choice;

extern player_map value_of_player_pos(void);

battle_result process_battle_turn(choice selection, int lock, int enemy_x, int enemy_y);
void battle_init(int enemy_y, int enemy_x);
void battle_screen_init(void);
void battle_log_init(void);
void battle_screen_draw(choice selection);
void battle_menu_draw(choice selection);

void reset_stats(void);
void stats_enemy(void);
int search_for_enemy(void);

int* get_location_of(items item);
bool* get_location_of_poison(void);
bool* get_location_of_boss_bool(void);
entity (*get_location_of_enemies(void))[MAX_NUMBER_OF_NPCS];
void call_battle_log(actions input_one, int input_two);