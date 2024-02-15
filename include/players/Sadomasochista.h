#pragma once

Player sadomasochista([](const std::vector<Spoluprace>& answers)
{
	return !answers.empty()&&!answers.back();
}, "Sadomasochista");