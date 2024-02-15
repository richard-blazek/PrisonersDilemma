#pragma once

struct StandardRating
{
	constexpr static int ratings[2][2] = {{1, 5}, {0, 3}};
	constexpr static int Rating(Answer thisPlayer, Answer otherPlayer)
	{
		return ratings[int(thisPlayer)][int(otherPlayer)];
	}
};