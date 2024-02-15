#pragma once

Player alternating([](const std::vector<Answer>& answers)
{
	return answers.size() % 2 == 0 ? Answer::Good : Answer::Evil;
}, "Alternating");