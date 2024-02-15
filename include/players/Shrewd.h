#pragma once

Player shrewd([](const std::vector<bool>& answers)
{
	return !answers.empty()&&implies(answers.size()>1, answers.back());
}, "Shrewd");