#pragma once

Player strategical([](const std::vector<bool>& answers)
{
	return implies(answers.size()>=2, answers[1]);
}, "Strategical");