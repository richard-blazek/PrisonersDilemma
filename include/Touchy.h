#pragma once

Player touchy([](const vector<bool>& answers)
{
	for(size_t i=1; i<answers.size(); ++i)
	{
		if(!answers[i-1]&&!answers[i])
		{
			return false;
		}
	}
	return answers.empty()||answers.back();
}, "Touchy");