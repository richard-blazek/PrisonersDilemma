#pragma once

Player crook([](const std::vector<Answer>& answers)
{
	if (answers.empty()) return Answer::Good;
	if (answers.size() % 2 == 1) return Answer::Evil;
	return answers.back();
}, "Crook");