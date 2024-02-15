#pragma once

Player tolerant([](const std::vector<Answer>& answers)
{
	for(size_t i=1; i<answers.size(); ++i)
	{
		if(answers[i-1] == Answer::Evil && answers[i] == Answer::Evil)
		{
			return Answer::Evil;
		}
	}
	return Answer::Good;
}, "Tolerant");