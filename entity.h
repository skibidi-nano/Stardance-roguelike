#ifndef ENTITY_H
#define ENTITY_H

typedef struct
{
    int max_hp;
    int current_hp;
    int attack_power;
} 
entity;

int health_bar_init(int current_hp, int max_hp);

void draw_health_bar(int health_bar, int max_hp);

#endif