#pragma once

Player randomizer([](const std::vector<Answer>& answers)
{
	return rand() % 2 == 0 ? Answer::Good : Answer::Evil;
}, "Random");