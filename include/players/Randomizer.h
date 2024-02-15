#pragma once

Player randomizer([](const std::vector<bool>& answers)
{
	return rand()%2==0;
}, "Random");