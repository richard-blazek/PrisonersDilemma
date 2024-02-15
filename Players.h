#pragma once

#include "include/StandardRating.h"

class Players
{
private:
	vector<Player> hraci;
public:
	Players()=default;
	Players(initializer_list<Player> init):hraci(init){}
	void Againist(Players& proti, uint16 loops)
	{
		for(auto& hrac:hraci)
		{
			for(auto& protihrac:proti.hraci)
			{
				vector<bool> answers1, answers2;
				answers1.reserve(loops);
				answers2.reserve(loops);
				for(uint16 i=0; i<loops; ++i)
				{
					bool odp=hrac.Answer(answers2);
					answers2.push_back(protihrac.Answer(answers1));
					answers1.push_back(odp);

					hrac.GiveRating<StandardRating>(answers1.back(), answers2.back());
				}
			}
		}
		sort(hraci.rbegin(), hraci.rend());
	}
	const Player& operator[](size_t i)const
	{
		return hraci[i];
	}
	size_t size()const
	{
		return hraci.size();
	}
};