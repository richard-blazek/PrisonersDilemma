#pragma once

using Strategy=std::function<bool(const std::vector<bool>&)>;

class Player
{
private:
	Strategy strategy;
	std::string name;
	int score=0;
public:
	Player(Strategy strategy, std::string name)
		:strategy(strategy), name(name) {}
    std::string StrategyName()const
    {
    	return name;
    }
    bool Answer(const std::vector<bool>& answers)const
    {
    	return strategy(answers);
    }
    bool operator<(const Player& second)
    {
    	return score<second.score;
    }
    template<typename RatingT>
    void GiveRating(bool i, bool other)
    {
    	score+=RatingT::Rating(i, other);
    }
    int Score()const
    {
    	return score;
    }
};