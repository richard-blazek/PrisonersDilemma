#pragma once

Player averager([](const std::vector<bool>& answers)
{
	return (size_t)count(answers.begin(), answers.end(), false)<=answers.size()/2;
}, "Averager");

Player patient_averager([](const std::vector<bool>& answers)
{
	return answers.size()<2||(size_t)count(answers.begin(), answers.end(), false)<=answers.size()/2;
}, "Patient averager");

Player deceitful_averager([](const std::vector<bool>& answers)
{
	return answers.size()>=2&&(size_t)count(answers.begin(), answers.end(), false)<=answers.size()/2;
}, "Deceitful averager");