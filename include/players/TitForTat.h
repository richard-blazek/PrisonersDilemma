#pragma once

Player tit_for_tat([](const std::vector<bool>& answers)
{
	return implies(!answers.empty(), answers.back());
}, "Tit for tat");

Player deceitfulTFT([](const std::vector<bool>& answers)
{
	return !answers.empty()&&answers.back();
}, "Deceitful tit for tat");