#pragma once

auto limited_forgiveness = [](int limit)
{
	return [=](const std::vector<bool>& answers)
	{
		return count(answers.begin(), answers.end(), false)<=limit;
	};
};

Player unforgiving(limited_forgiveness(0), "Unforgiving");
Player forgives_once(limited_forgiveness(1), "Forgives once");
Player forgives_twice(limited_forgiveness(2), "Forgives twice");