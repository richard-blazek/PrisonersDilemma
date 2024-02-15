#include <algorithm>
#include <functional>
#include <iostream>
#include <ctime>
#include <string>
#include "mylibraries/func.h"
#include "gui/gui.h"

using namespace std;

bool implies(bool a, bool b)
{
	return !a||b;
}

#include "Player.h"
#include "Players.h"

#include "Alternating.h"
#include "Averager.h"
#include "Brainy.h"
#include "Crook.h"
#include "Forgiving.h"
#include "Friendly.h"
#include "Masochist.h"
#include "Random.h"
#include "Shrewd.h"
#include "Strategical.h"
#include "TitFor2Tats.h"
#include "TitForTat.h"
#include "Tolerant.h"
#include "Touchy.h"
#include "Traitor.h"
#include "Unforgiving.h"

int main()
{
	srand(time(0));
    Players players_copy={alternating, averager, brainy, crook, forgiving, friendly, shrewd, strategical,
		tit_for_2tats, deceitfulTF2T, tit_for_tat, deceitfulTFT, tolerant, touchy, traitor, unforgiving,
		forgives_once, forgives_twice, patient_averager, deceitful_averager
    };
    SDL::Init sdl;
    SDL::Font font("font.ttf", 12);
    SDL::Window window("Game theory", SDL::Rect(50,50, 800,600), SDL::Window::Flags::Resizable);
    SDL::Renderer rend(window);
    int rounds=0;
    bool repeat=true;
    while(repeat)
	{
		Players players=players_copy;
		players.Againist(players, rounds+1);
		rend.Repaint(SDL::Color(0,0,0));
		rend.Draw(font, "Round count: "+to_string(rounds+1), SDL::Color(255,255,255), SDL::Rect(window.Size().x/2,0, window.Size().x/2, 24));
		for(size_t i=0; i<players.size(); ++i)
		{
			int32 height=players[i].Score()*window.Size().y/players[0].Score();
			SDL::Rect dst_area(i*window.Size().x/players.size(), window.Size().y-height, window.Size().x/players.size()*10/11, height);
			rend.Draw(dst_area, SDL::Color(i*255/players.size(),255-i*255/players.size(),0));
			auto text_img=font.Render(players[i].StrategyName(), SDL::Color(0,0,0)).TurnLeft();
			rend.Draw(text_img, text_img.Size(), SDL::Rect(dst_area.x+dst_area.w/4, window.Size().y-text_img.Size().y-5, text_img.Size()));
			if(dst_area.Encloses(SDL::Cursor::Position()))
			{
				rend.DrawBorder(dst_area, SDL::Color(64,64,255));
				rend.Draw(font, to_string(players[i].Score()*1000/players[0].Score()), SDL::Color(0,0,0), SDL::Rect(dst_area.LeftUp(), dst_area.w, text_img.Size().x)+SDL::Point(0, 5));
			}
		}
		for(auto& event:SDL::events::Handler())
		{
			if(event.Type()==SDL::events::Type::Quit)
			{
				repeat=false;
			}
			else if(event.Type()==SDL::events::Type::MouseWheel)
			{
				rounds+=50+event.MouseWheel().Move.y;
				rounds%=50;
			}
		}
		rend.Show();
		SDL::Wait(50);
	}
    return 0;
}