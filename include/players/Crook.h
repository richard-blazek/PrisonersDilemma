#pragma once

Player crook([](const std::vector<bool>& answers)
{
	return answers.empty()||(answers.size()%2==0&&answers.back());
}, "Crook");