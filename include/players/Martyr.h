#pragma once

Player martyr([](const std::vector<Answer>& answers)
{
	return answers.empty() ? Answer::Good : !answers.back();
}, "Martyr");

Player cruel_martyr([](const std::vector<Answer>& answers)
{
	return answers.empty() ? Answer::Evil : !answers.back();
}, "Cruel martyr");