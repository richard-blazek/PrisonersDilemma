#pragma once

Player alternating([](const std::vector<bool>& answers)
{
	return answers.size()%2==0;
}, "Alternating");