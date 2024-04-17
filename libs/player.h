#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdbool.h>

typedef struct player
{
    bool (*strategy)(bool *, int);
    const char *name;
    int score;
} player;

void clear_score(player *players, int count);
void play_game(player *players, int count, int loops);

#endif
