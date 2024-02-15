#pragma once

Player strategical([](const std::vector<Answer>& answers)
{
	return answers.size() >= 2 ? answers[1] : Answer::Good;
}, "Strategical");