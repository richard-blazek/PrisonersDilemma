#pragma once

Player forgiving([](const std::vector<Answer>& answers)
{
	if (answers.empty() || answers.size() % 2 == 1) return Answer::Good;
	return answers.back();
}, "Forgiving");