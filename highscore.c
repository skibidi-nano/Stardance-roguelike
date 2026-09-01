#include "highscore.h"
#include <stdio.h>

static int current_score = 0;

int score_tracking(int score_update)
{
    current_score += score_update;
    return current_score;
}
void score_register(void)
{
    FILE *input = fopen("highscore.txt", "r+b");
    if (input == NULL)
    {
        return;
    }

    int current_highscore = 0;
    
    if (fread(&current_highscore, sizeof(int), 1, input) != 1) 
    {
        current_highscore = 0; // default if file is empty
    }

    if (current_highscore < current_score)
    {
        current_highscore = current_score;
        fseek(input, 0, SEEK_SET); 
        fwrite(&current_highscore, sizeof(int), 1, input);
    }

    fclose(input);

}

int get_highscore(void)
{
    FILE *input = fopen("highscore.txt", "r+b");
    if (input == NULL)
    {
        return -1;
    }

    int current_highscore = 0;
    
    if (fread(&current_highscore, sizeof(int), 1, input) != 1) 
    {
        current_highscore = 0; // default if file is empty
    }

    fclose(input);

    return current_highscore;
}