#include "iqaitem.h"

IQAItem::IQAItem(ISection *section)
{
    setSection(section);
}

bool IQAItem::parseJson(const QJsonObject &json)
{
    m_question = JsonHelper::readStringValue("question", json);
    m_variableName = JsonHelper::readStringValue("variable", json);
}

QString IQAItem::question() const
{
    return m_question;
}

ISection *IQAItem::section() const
{
    return m_section.data();
}

void IQAItem::setSection(ISection *section)
{
    m_section.reset(section);
}

QString IQAItem::variableName() const
{
    return m_variableName;
}
