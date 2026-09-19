#ifndef ENTITY_H
#define ENTITY_H

typedef enum 
{
    ADDRESS,
    MODULO,
    BOSS,
    LAST_ENUM
}
type;

typedef struct
{
    int max_hp;
    int current_hp;
    int attack_power;
} 
entity;

void draw_health_bar(int current_hp, int max_hp, int y);

void draw_current_strength(int current_strength, int y);
void draw_current_hp(int current_hp, int y);

#endif