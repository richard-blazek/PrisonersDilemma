#pragma once

Player strategical([](const vector<bool>& answers)
{
	return implies(answers.size()>=2, answers[1]);
}, "Strategical");