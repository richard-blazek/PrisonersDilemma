#pragma once

Player tit_for_tat([](const std::vector<Answer>& answers)
{
	return answers.empty() ? Answer::Good : answers.back();
}, "Tit for tat");

Player deceitfulTFT([](const std::vector<Answer>& answers)
{
	return answers.empty() ? Answer::Evil : answers.back();
}, "Deceitful tit for tat");