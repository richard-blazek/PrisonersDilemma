#pragma once

struct StandardRating
{
	constexpr static int ratings[2][2]={{1,5}, {0,3}};
	constexpr static int Rating(bool thisPlayer, bool otherPlayer)
	{
    	return ratings[thisPlayer][otherPlayer];
	}
};