#pragma once

Player random([](const vector<bool>& answers)
{
	return rand()%2==0;
}, "Random");