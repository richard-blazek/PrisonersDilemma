#pragma once

#include "StandardRating.h"

class Players
{
private:
	std::vector<Player> players;
public:
	Players()=default;
	Players(std::initializer_list<Player> players):players(players){}
	void Againist(Players& proti, int loops)
	{
		for(auto& hrac:players)
		{
			for(auto& protihrac:proti.players)
			{
				std::vector<bool> answers1, answers2;
				answers1.reserve(loops);
				answers2.reserve(loops);
				for(int i=0; i<loops; ++i)
				{
					bool odp=hrac.Answer(answers2);
					answers2.push_back(protihrac.Answer(answers1));
					answers1.push_back(odp);

					hrac.GiveRating<StandardRating>(answers1.back(), answers2.back());
				}
			}
		}
		sort(players.rbegin(), players.rend());
	}
	const Player& operator[](int i)const
	{
		return players[i];
	}
	int size()const
	{
		return players.size();
	}
};