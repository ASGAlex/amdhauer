#include "qaitemfixed.h"

QAItemFixed::QAItemFixed(ISection *section)
    :IQAItem(section)
{
}

QAItemFixed::~QAItemFixed()
{
}

bool QAItemFixed::parseJson(const QJsonObject& json)
{
    IQAItem::parseJson(json);

    m_answerVariants = JsonHelper::readStringArray("answerVariants", json);
    m_answerScore = JsonHelper::readIntArray("answerScore", json);

    return true;
}

const QStringList &QAItemFixed::answerVariants() const
{
    return m_answerVariants;
}

const QList<int>& QAItemFixed::answerScore() const
{
    return m_answerScore;
}
