#pragma once

Player sadomasochista([](const vector<Spoluprace>& answers)
{
	return !answers.empty()&&!answers.back();
}, "Sadomasochista");