#ifndef RATEDANSWER_H
#define RATEDANSWER_H

#include <QSet>
#include <QString>
#include <QDataStream>
#include <QJsonObject>

class RatedAnswer {
public:
    RatedAnswer();
    RatedAnswer(QString answer);
    RatedAnswer(int score, QString answer);
    RatedAnswer(const RatedAnswer &ratedAnswer);

    QJsonObject toJsonObject() const;

    QString answer() const;
    int score() const;

    RatedAnswer &operator=(const RatedAnswer &ratedAnswer);

    friend QDataStream &operator>>(QDataStream &in, RatedAnswer &ratedAnswer);

    bool operator==(const QString &answerString) const {
        return m_answer == answerString;
    }


protected:
    QString m_answer;
    int m_score;
};

inline QDataStream &operator<<(QDataStream &out,const RatedAnswer &ratedAnswer){
    out << ratedAnswer.score() << ratedAnswer.answer();
    return out;
}
inline QDataStream &operator>>(QDataStream &in, RatedAnswer &ratedAnswer) {
    int score;
    QString answer;
    in >> score >> answer;

    ratedAnswer.m_answer = answer;
    ratedAnswer.m_score = score;

    return in;

}

inline bool operator==(const RatedAnswer &ra1, const RatedAnswer &ra2)
{
    return ra1.answer() == ra2.answer();
}


inline uint qHash(const RatedAnswer &key, uint seed)
{
    return qHash(key.answer(), seed);
}
#endif // RATEDANSWER_H
