#pragma once

Player brainy([](const std::vector<bool>& answers)
{
	switch(answers.size())
	{
	case 0:
		return false;
	case 1:
	case 2:
	case 3:
		return true;
	default:
		return implies(answers[1], answers.size()%2==1)&&implies(answers[0]&&!answers[1]&&answers[2]&&!answers[3], false)&&answers.back();
	}
}, "Brainy");