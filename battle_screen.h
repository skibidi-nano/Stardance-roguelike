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
    BATTLE_ERROR
} battle_result;

typedef enum
{
    ATTACK,
    ITEM,
    RUN
} choice;

battle_result process_battle_turn(int init_mode, choice selection, int lock, int enemy_x, int enemy_y);
void battle_init(void);
void battle_screen_init(void);
void battle_screen_draw(choice selection);
void battle_menu_draw(choice selection);
void reset_stats(void);