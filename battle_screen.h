typedef enum
{
    TURN_PLAYER,
    TURN_ENEMY,
} turn_player;

int process_battle_turn(int init_mode, int selection, int lock);
void battle_init(void);
void battle_screen_init(void);
void battle_screen_draw();
void battle_menu_draw(int selection);