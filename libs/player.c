#include "player.h"
#include <stdlib.h>

static int rating[2][2] = {{1, 5}, {0, 3}};

static int compare_players(const void *vp1, const void *vp2)
{
	player *p1 = (player *)vp1, *p2 = (player *)vp2;
	return p2->score - p1->score;
}

static void sort_players(player *players, int count)
{
	qsort(players, count, sizeof(player), compare_players);
}

void clear_score(player *players, int count)
{
	for (int i = 0; i < count; ++i)
	{
		players[i].score = 0;
	}
}

void play_game(player *players, int count, int loops)
{
	bool *answers1 = (bool *)malloc(sizeof(bool) * loops);
	bool *answers2 = (bool *)malloc(sizeof(bool) * loops);
	for (int i = 0; i < count; ++i)
	{
		for (int j = 0; j < count; ++j)
		{
			for (int k = 0; k < loops; ++k)
			{
				answers1[k] = players[i].strategy(answers2, k);
				answers2[k] = players[j].strategy(answers1, k);
				players[i].score += rating[answers1[k]][answers2[k]];
			}
		}
	}
	free(answers1);
	free(answers2);

	sort_players(players, count);
}
