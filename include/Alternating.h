#pragma once

Player alternating([](const vector<bool>& answers)
{
	return answers.size()%2==0;
}, "Alternating");