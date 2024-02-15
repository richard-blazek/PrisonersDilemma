#pragma once

Player tit_for_2tats([](const std::vector<Answer>& answers)
{
	if (answers.size() < 2) return Answer::Good;
	if (answers.back() == Answer::Evil && answers[answers.size() - 2] == Answer::Evil) return Answer::Evil;
	return Answer::Good;
}, "Tit for 2 tats");

Player deceitfulTF2T([](const std::vector<Answer>& answers)
{
	if (answers.size() < 2) return Answer::Evil;
	if (answers.back() == Answer::Evil && answers[answers.size() - 2] == Answer::Evil) return Answer::Evil;
	return Answer::Good;
}, "Deceitful tit for 2 tats");