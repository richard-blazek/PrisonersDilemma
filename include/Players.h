#pragma once

#include "StandardRating.h"

class Players
{
private:
	std::vector<Player> players;

public:
	Players() = default;
	Players(std::initializer_list<Player> players) : players(players) {}
	void Againist(Players &opponents, int loops)
	{
		for (auto &player : players)
		{
			for (auto &opponent : opponents.players)
			{
				std::vector<Answer> answers1, answers2;
				answers1.reserve(loops);
				answers2.reserve(loops);
				for (int i = 0; i < loops; ++i)
				{
					Answer ans = player.SayAnswer(answers2);
					answers2.push_back(opponent.SayAnswer(answers1));
					answers1.push_back(ans);

					player.GiveRating<StandardRating>(answers1.back(), answers2.back());
				}
			}
		}
		sort(players.rbegin(), players.rend());
	}
	const Player &operator[](int i) const
	{
		return players[i];
	}
	int size() const
	{
		return players.size();
	}
};