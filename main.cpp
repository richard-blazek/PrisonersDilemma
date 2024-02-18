#include <algorithm>
#include <functional>
#include <iostream>
#include <ctime>
#include <string>
#include <SDL2/SDL_ttf.h>

#include "include/Answer.h"
#include "include/Player.h"
#include "include/Players.h"
#include "include/Output.h"
#include "include/Strategies.h"

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
				rounds += 100 + event.wheel.y;
				rounds %= 100;
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