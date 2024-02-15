#pragma once

auto limited_forgiveness = [](int limit)
{
	return [=](const std::vector<Answer>& answers)
	{
		return count(answers.begin(), answers.end(), Answer::Evil) <= limit ? Answer::Good : Answer::Evil;
	};
};

Player unforgiving(limited_forgiveness(0), "Unforgiving");
Player forgives_once(limited_forgiveness(1), "Forgives once");
Player forgives_twice(limited_forgiveness(2), "Forgives twice");