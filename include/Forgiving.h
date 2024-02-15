#pragma once

Player forgiving([](const vector<bool>& answers)
{
	return answers.empty()||(answers.size()%2==1||answers.back());
}, "Forgiving");