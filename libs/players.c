#include "players.h"
#include <stdlib.h>

static int betrayals(bool *answers, int count)
{
    int total = 0;
    for (int i = 0; i < count; ++i)
    {
        total += !answers[i];
    }
    return total;
}

static bool clever(bool *answers, int count)
{
    if (count == 0) return false;
    if (count <= 3) return true;
    if (count % 2 == 0 && answers[1]) return false;
    if (answers[0] && !answers[1] && answers[2] && !answers[3]) return false;
    return answers[count - 1];
}

static bool tolerant(bool *answers, int count)
{
    bool tolerate = true;
    for (int i = 1; i < count && tolerate; ++i)
    {
        tolerate &= answers[i - 1] || answers[i];
    }
    return tolerate;
}

static bool agreeable(bool *answers, int count) { return true; }
static bool averager(bool *answers, int count) { return betrayals(answers, count) <= count / 2; }
static bool randomiser(bool *answers, int count) { return rand() % 2 == 0; }
static bool crook(bool *answers, int count) { return count == 0 || (count % 2 == 0 && answers[count - 1]); }
static bool evil_avg(bool *answers, int count) { return count >= 2 && averager(answers, count); }
static bool evil_negative(bool *answers, int count) { return count != 0 && !answers[count - 1]; }
static bool evil_tf2t(bool *answers, int count) { return count >= 2 && (answers[count - 1] || answers[count - 2]); }
static bool evil_tf1t(bool *answers, int count) { return count != 0 && answers[count - 1]; }
static bool flipflop(bool *answers, int count) { return count % 2 == 0; }
static bool forgive0(bool *answers, int count) { return betrayals(answers, count) == 0; }
static bool forgive1(bool *answers, int count) { return betrayals(answers, count) <= 1; }
static bool forgive2(bool *answers, int count) { return betrayals(answers, count) <= 2; }
static bool historic(bool *answers, int count) { return count < 2 || answers[1]; }
static bool negative(bool *answers, int count) { return count == 0 || !answers[count - 1]; }
static bool nice_avg(bool *answers, int count) { return count < 2 || averager(answers, count); }
static bool evil(bool *answers, int count) { return false; }
static bool provocateur(bool *answers, int count) { return count != 0 && (count == 1 || answers[count - 1]); }
static bool tf1t(bool *answers, int count) { return count == 0 || answers[count - 1]; }
static bool tf2t(bool *answers, int count) { return count < 2 || answers[count - 1] || answers[count - 2]; }
static bool touchy(bool *answers, int count) { return tf1t(answers, count) && tolerant(answers, count); }

static player players_array[] =
{
    {clever, "Clever"}, {tolerant, "Tolerant"}, {agreeable, "Agreeable"}, {averager, "Averager"},
    /*{randomiser, "Randomiser"},*/ {crook, "Crook"}, {evil_avg, "Evil Averager"}, /*{evil_negative, "Evil Negative"},*/
    {evil_tf2t, "Evil Tit for 2 Tats"}, {evil_tf1t, "Evil Tit for Tat"}, {flipflop, "Flip-flopper"}, {forgive0, "Unforgiving"},
    {forgive1, "Forgiving once"}, {forgive2, "Forigving twice"}, {historic, "Historic"}, /*{negative, "Negative"},*/
    {nice_avg, "Nice Averager"}, {evil, "Evil"}, {provocateur, "Provocateur"}, {tf1t, "Tit for Tat"},
    {tf2t, "Tit for 2 Tats"}, {touchy, "Touchy"}
};

player * const players = players_array;
const int players_len = sizeof (players_array) / sizeof (player);
