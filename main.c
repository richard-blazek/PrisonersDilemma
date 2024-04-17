#include "libs/player.h"
#include "libs/players.h"
#include "libs/io.h"

#include <stdlib.h>
#include <time.h>

int main()
{
	srand(time(0));
	io_init();

	int rounds = 0;
	bool repeat = true;

	while (io_loop())
	{
		clear_score(players, players_len);
		play_game(players, players_len, rounds + 1);
		io_draw(players, players_len, rounds + 1);

		rounds = (rounds + io_mouse_scroll() + 128) & 127;
	}
	
	io_quit();
	return 0;
}