#pragma once

Player tit_for_2tats([](const std::vector<bool>& answers)
{
	return answers.empty()||(answers.back()&&(answers.size()<2||answers[answers.size()-2]));
}, "Tit for 2 tats");

Player deceitfulTF2T([](const std::vector<bool>& answers)
{
	return answers.size()>=2&&answers.back()&&answers[answers.size()-2];
}, "Deceitful tit for 2 tats");