#include "souterrain.h"

#define BUF_SIZE 1000

int main(void)
{
    level_t levels[4] = {
        load_level("lvl01.txt"),
        load_level("lvl01.txt"),
        load_level("lvl01.txt"),
        load_level("lvl01.txt")
    };
    player_t player = player_set();

    run_levels(4, levels, &player);

    return 0;
}