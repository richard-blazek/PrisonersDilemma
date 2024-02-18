#pragma once

Player alternating([](const std::vector<Answer>& answers)
{
	return answers.size() % 2 == 0 ? Answer::Good : Answer::Evil;
}, "Alternating");

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

Player brainy([](const std::vector<Answer>& answers)
{
	switch(answers.size())
	{
	case 0:
		return Answer::Evil;
	case 1:
	case 2:
	case 3:
		return Answer::Good;
	default:
		return (answers.size() % 2 == 0 ? !answers[1] : Answer::Good) && !(answers[0] && !answers[1] && answers[2] && !answers[3]) && answers.back();
	}
}, "Brainy");

Player crook([](const std::vector<Answer>& answers)
{
	if (answers.empty()) return Answer::Good;
	if (answers.size() % 2 == 1) return Answer::Evil;
	return answers.back();
}, "Crook");

Player forgiving([](const std::vector<Answer>& answers)
{
	if (answers.empty() || answers.size() % 2 == 1) return Answer::Good;
	return answers.back();
}, "Forgiving");

Player friendly([](const std::vector<Answer>& x){return Answer::Good;}, "Friendly");

Player martyr([](const std::vector<Answer>& answers)
{
	return answers.empty() ? Answer::Good : !answers.back();
}, "Martyr");

Player cruel_martyr([](const std::vector<Answer>& answers)
{
	return answers.empty() ? Answer::Evil : !answers.back();
}, "Cruel martyr");

Player randomizer([](const std::vector<Answer>& answers)
{
	return rand() % 2 == 0 ? Answer::Good : Answer::Evil;
}, "Random");

Player shrewd([](const std::vector<Answer>& answers)
{
	if (answers.empty()) return Answer::Evil;
	if (answers.size() == 1) return Answer::Good;
	return answers.back();
}, "Shrewd");

Player strategical([](const std::vector<Answer>& answers)
{
	return answers.size() >= 2 ? answers[1] : Answer::Good;
}, "Strategical");

Player tit_for_2tats([](const std::vector<Answer>& answers)
{
	if (answers.size() < 2) return Answer::Good;
	if (answers.back() == Answer::Evil && answers[answers.size() - 2] == Answer::Evil) return Answer::Evil;
	return Answer::Good;
}, "Tit for 2 tats");

Player deceitfulTF2T([](const std::vector<Answer>& answers)
{
	if (answers.size() < 2) return Answer::Evil;
	if (answers.back() == Answer::Evil && answers[answers.size() - 2] == Answer::Evil) return Answer::Evil;
	return Answer::Good;
}, "Deceitful tit for 2 tats");

Player tit_for_tat([](const std::vector<Answer>& answers)
{
	return answers.empty() ? Answer::Good : answers.back();
}, "Tit for tat");

Player deceitfulTFT([](const std::vector<Answer>& answers)
{
	return answers.empty() ? Answer::Evil : answers.back();
}, "Deceitful tit for tat");

Player tolerant([](const std::vector<Answer>& answers)
{
	for(size_t i=1; i<answers.size(); ++i)
	{
		if(answers[i-1] == Answer::Evil && answers[i] == Answer::Evil)
		{
			return Answer::Evil;
		}
	}
	return Answer::Good;
}, "Tolerant");

Player touchy([](const std::vector<Answer>& answers)
{
	if (answers.empty()) return Answer::Good;
	for(size_t i=1; i<answers.size(); ++i)
	{
		if(answers[i-1] == Answer::Evil && answers[i] == Answer::Evil)
		{
			return Answer::Evil;
		}
	}
	return answers.back();
}, "Touchy");

Player traitor([](const std::vector<Answer>& x){return Answer::Evil;}, "Traitor");

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
