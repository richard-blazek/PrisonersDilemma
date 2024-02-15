#pragma once

Player brainy([](const std::vector<Answer>& answers)
{
	switch(answers.size())
	{
	case 0:
		return Answer::Evil;
	case 1:
	case 2:
	case 3:
		return Answer::Good;
	default:
		return (answers.size() % 2 == 0 ? !answers[1] : Answer::Good) && !(answers[0] && !answers[1] && answers[2] && !answers[3]) && answers.back();
	}
}, "Brainy");