#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "console.h"
#include "souterrain.h"

/* PRIVATE FUNCTIONS */

int file_read(char *file_path, int buf_dst_width, int buf_dst_height, char buf_dst[buf_dst_height][buf_dst_width], int *width, int *height)
{
    FILE *fp;
    char c;
    int read_char_counter = 0, x = 0;

    if (!file_path || !buf_dst || buf_dst_width <= 0 || buf_dst_height <= 0)
        return -1;

    fp = fopen(file_path, "r");
    if (!fp)
        return -1;

    *width = 0;
    *height = 0;
    while (fscanf(fp, "%c", &c) == 1)
    {
        if (buf_dst_width * buf_dst_height > read_char_counter)
        {
            if (c != '\n')
            {
                buf_dst[*height][x] = c;
                x++;
            }
            else
            {
                if (*width && x != *width)
                    break;
                else
                    *width = x;
                x = 0;
                (*height)++;
            }
            read_char_counter++;
        }
        else
            break;
    }
    (*height)++;

    fclose(fp);

    return read_char_counter >= 0 ? read_char_counter : -1;
}

int file_write(char *file_path, int buf_src_width, int buf_src_height, char buf_src[buf_src_height][buf_src_width], int width, int height)
{
    FILE *fp;
    int x, y;

    if (!file_path || !buf_src || buf_src_width <= 0 || buf_src_height <= 0 || width <= 0 || height <= 0)
        return -1;

    fp = fopen(file_path, "w");
    if (!fp)
        return -1;

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
            fprintf(fp, "%c", buf_src[y][x]);
        fprintf(fp, "\n");
    }

    fclose(fp);

    return 0;
}

void print_buf(char *buf, int buf_length)
{
    int i;

    for (i = 0; i < buf_length; i++)
        printf("%c", buf[i]);
    printf("\n");

    return;
}

int print_level(level_t *level, player_t *player, int level_nb)
{
    int y;

    if (level->width <= 0 || level->height <= 0)
        return -1;

    printf("LEVEL %d\n\nPlayer: %s\n - Health : %d\n - Gold : %d\n - Dexterity : %d\n\n", level_nb + 1, player->name, player->health, player->gold, player->dexterity);
    for (y = 0; y < level->height; y++)
        print_buf(level->data[y], level->width);

    return 0;
}

#define WALL        '|'
#define WALL_EDGE   '+'
#define PATH        ' '
#define LEVEL_EXIT  '0'

int move_player(level_t *level, player_t *player, int player_new_pos_x, int player_new_pos_y, int *level_counter)
{
    if (!level || !player || player_new_pos_x < 0 || player_new_pos_x > level->width || player_new_pos_y < 0 || player_new_pos_y > level->height)
        return -1;

    switch (level->data[player_new_pos_y][player_new_pos_x])
    {
        default:
        case WALL:
        case WALL_EDGE:
            break;
        case PATH:
            level->data[player->y][player->x] = ' ';
            level->data[player_new_pos_y][player_new_pos_x] = '@';
            player->x = player_new_pos_x;
            player->y = player_new_pos_y;
            break;
        case LEVEL_EXIT:
            (*level_counter)++;
            break;
    }

    return 0;
}

#define QUIT        'e'
#define MOVE_UP     'z'
#define MOVE_DOWN   's'
#define MOVE_LEFT   'q'
#define MOVE_RIGHT  'd'

int run_level(level_t *level, player_t *player, int start_player_pos_x, int start_player_pos_y, int *level_counter)
{
    char c;
    int current_level = *level_counter;

    if (!level || !player || start_player_pos_x < 0 || start_player_pos_x > level->width || start_player_pos_y < 0 || start_player_pos_y > level->height)
        return -1;

    efface_ecran();
    desactive_curseur();

    player->x = start_player_pos_x;
    player->y = start_player_pos_y;
    move_player(level, player, player->x, player->y, level_counter);
    print_level(level, player,*level_counter);

    while ((c = recupcar()) != QUIT)
    {
        switch (c)
        {
            case MOVE_UP:
                move_player(level, player, player->x, player->y - 1, level_counter);
                efface_ecran();
                resetterminal();
                print_level(level, player,*level_counter);
                break;
            case MOVE_DOWN:
                move_player(level, player, player->x, player->y + 1, level_counter);
                efface_ecran();
                resetterminal();
                print_level(level, player,*level_counter);
                break;
            case MOVE_LEFT:
                move_player(level, player, player->x - 1, player->y, level_counter);
                efface_ecran();
                resetterminal();
                print_level(level, player,*level_counter);
                break;
            case MOVE_RIGHT:
                move_player(level, player, player->x + 1, player->y, level_counter);
                efface_ecran();
                resetterminal();
                print_level(level, player,*level_counter);
                break;
        }

        if (*level_counter != current_level)
            break;
    }

    efface_ecran();
    active_curseur();
    resetterminal();

    return 0;
}

int random_number(int min, int max)
{
    srand(time(NULL));
    return rand() % (max - min + 1) + min;
}

/* PUBLIC FUNCTIONS */

level_t load_level(char *level_file_path)
{
    level_t level;

    if(file_read(level_file_path, LEVEL_MAX_WIDTH, LEVEL_MAX_HEIGHT, level.data, &level.width, &level.height) == -1)
        return (struct level){-1, -1, {{0}}};

    if (level.width < 20 || level.width > 40 || level.height < 20 || level.height > 40)
        return (struct level){-1, -1, {{0}}};

    return level;
}

int run_levels(int nb_levels, level_t levels[nb_levels], player_t *player)
{
    int level_counter = 0;

    if (nb_levels <= 0 || !levels || !player)
        return -1;

    while (level_counter < nb_levels)
        run_level(&levels[level_counter], player, 1, 1, &level_counter);

    return 0;
}

player_t player_set(void)
{
    player_t player;

    printf("Nom du joueur : ");
    scanf("%8s", player.name);

    player.health = random_number(2, 10);
    player.moves = 0;
    player.gold = 0;
    player.dexterity = random_number(1, 7);

    return player;
}

// Étape 6