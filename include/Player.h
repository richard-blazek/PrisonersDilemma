#pragma once

using Strategy = std::function<Answer(const std::vector<Answer> &)>;

class Player
{
private:
  Strategy strategy;
  std::string name;
  int score = 0;

public:
  Player(Strategy strategy, std::string name)
      : strategy(strategy), name(name) {}
  std::string StrategyName() const
  {
    return name;
  }
  Answer SayAnswer(const std::vector<Answer> &answers) const
  {
    return strategy(answers);
  }
  bool operator<(const Player &second)
  {
    return score < second.score;
  }
  template <typename RatingT>
  void GiveRating(Answer i, Answer other)
  {
    score += RatingT::Rating(i, other);
  }
  int Score() const
  {
    return score;
  }
};