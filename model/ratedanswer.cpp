#include "ratedanswer.h"


RatedAnswer::RatedAnswer()
{

}

RatedAnswer::RatedAnswer(QString answer)
{
    m_score = 0;
    m_answer = answer;
}

RatedAnswer::RatedAnswer(int score, QString answer)
{
    m_score = score;
    m_answer = answer;
}

RatedAnswer::RatedAnswer(const RatedAnswer &ratedAnswer)
{
    m_score = ratedAnswer.score();
    m_answer = ratedAnswer.answer();
}

QJsonObject RatedAnswer::toJsonObject() const
{
    QJsonObject result;
    result["score"] = m_score;
    result["answer"] = m_answer;
    return result;
}

QString RatedAnswer::answer() const
{
    return m_answer;
}

int RatedAnswer::score() const
{
    return m_score;
}


RatedAnswer &RatedAnswer::operator=(const RatedAnswer &ratedAnswer)
{
    m_score = ratedAnswer.score();
    m_answer = ratedAnswer.answer();
    return *this;
}
