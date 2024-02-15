#include <algorithm>
#include <functional>
#include <iostream>
#include <ctime>
#include <string>
#include <SDL_ttf.h>

#include "include/Answer.h"
#include "include/Player.h"
#include "include/Players.h"
#include "include/Output.h"

#include "include/players/Alternating.h"
#include "include/players/Averager.h"
#include "include/players/Brainy.h"
#include "include/players/Crook.h"
#include "include/players/Forgiving.h"
#include "include/players/Friendly.h"
#include "include/players/Martyr.h"
#include "include/players/Randomizer.h"
#include "include/players/Shrewd.h"
#include "include/players/Strategical.h"
#include "include/players/TitFor2Tats.h"
#include "include/players/TitForTat.h"
#include "include/players/Tolerant.h"
#include "include/players/Touchy.h"
#include "include/players/Traitor.h"
#include "include/players/Unforgiving.h"

void Main()
{
	Output output;

	Players players_copy = {
		alternating, averager, brainy, crook, forgiving, friendly, shrewd, strategical,
		tit_for_2tats, deceitfulTF2T, tit_for_tat, deceitfulTFT, tolerant, touchy, traitor, unforgiving,
		forgives_once, forgives_twice, patient_averager, deceitful_averager};

	int rounds = 0;
	bool repeat = true;
	SDL_Event event;
	while (repeat)
	{
		Players players = players_copy;
		players.Againist(players, rounds + 1);

		output.Draw(players, rounds);

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				repeat = false;
			}
			else if (event.type == SDL_MOUSEWHEEL)
			{
				rounds += 50 + event.wheel.y;
				rounds %= 50;
			}
		}
		SDL_Delay(50);
	}
}

int main()
{
	srand(time(0));
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	Main();
	
	TTF_Quit();
	SDL_Quit();
	return 0;
}