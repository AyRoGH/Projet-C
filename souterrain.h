#ifndef __SOUTERRAIN_H
#define __SOUTERRAIN_H

#define LEVEL_MAX_WIDTH 40
#define LEVEL_MAX_HEIGHT 40

typedef struct level
{
    int width;
    int height;
    char data[LEVEL_MAX_HEIGHT][LEVEL_MAX_WIDTH];
} level_t;

#define PLAYER_NAME_LENGHT 8

typedef struct player
{
    int x;
    int y;
    int health;
    int moves;
    int gold;
    int dexterity;
    char name[PLAYER_NAME_LENGHT + 1];
} player_t;

level_t load_level(char *level_file_path);
int run_levels(int nb_levels, level_t levels[nb_levels], player_t *player);
player_t player_set(void);

#endif /* __SOUTERRAIN_H */