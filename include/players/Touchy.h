#pragma once

Player touchy([](const std::vector<Answer>& answers)
{
	if (answers.empty()) return Answer::Good;
	for(size_t i=1; i<answers.size(); ++i)
	{
		if(answers[i-1] == Answer::Evil && answers[i] == Answer::Evil)
		{
			return Answer::Evil;
		}
	}
	return answers.back();
}, "Touchy");