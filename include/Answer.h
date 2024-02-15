#pragma once

enum class Answer
{
    Evil,
    Good
};

Answer operator!(Answer a)
{
    return a == Answer::Good ? Answer::Evil : Answer::Good;
}

Answer operator&&(Answer a, Answer b)
{
    return a == Answer::Good && b == Answer::Good ? Answer::Good : Answer::Evil;
}

Answer operator||(Answer a, Answer b)
{
    return a == Answer::Good || b == Answer::Good ? Answer::Good : Answer::Evil;
}

Answer implies(Answer a, Answer b)
{
	return !a || b;
}