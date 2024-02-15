#pragma once

Player shrewd([](const std::vector<Answer>& answers)
{
	if (answers.empty()) return Answer::Evil;
	if (answers.size() == 1) return Answer::Good;
	return answers.back();
}, "Shrewd");