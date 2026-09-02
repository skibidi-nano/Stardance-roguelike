typedef struct 
{
    int x;
    int y;
    int width;
    int height;
    int door_x;
    int door_y;
} room;

room random_room_gen(void);
int get_random_int(int min, int max);
