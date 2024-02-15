#pragma once

Player averager([](const std::vector<Answer>& answers)
{
	size_t count = std::count(answers.begin(), answers.end(), Answer::Evil);
	return count <= answers.size() / 2 ? Answer::Good : Answer::Evil;
}, "Averager");

Player patient_averager([](const std::vector<Answer>& answers)
{
	return answers.size() < 2 ? Answer::Good : averager.SayAnswer(answers);
}, "Patient averager");

Player deceitful_averager([](const std::vector<Answer>& answers)
{
	return answers.size() < 2 ? Answer::Evil : averager.SayAnswer(answers);
}, "Deceitful averager");