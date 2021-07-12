#pragma once

using Strategy=std::function<bool(const vector<bool>&)>;

class Player
{
private:
	Strategy strategy;
	string name;
	uint32 score=0;
public:
	Player(Strategy strategy, string name)
		:strategy(strategy), name(name) {}
    string StrategyName()const
    {
    	return name;
    }
    bool Answer(const vector<bool>& answers)const
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
    uint32 Score()const
    {
    	return score;
    }
};