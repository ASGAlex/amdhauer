#ifndef IQAITEM_H
#define IQAITEM_H

#include <QString>
#include <QRegExp>
#include <QJsonObject>
#include <QSharedPointer>
#include <QIntValidator>
#include <QRegExpValidator>
#include "../helper/jsonhelper.h"

class ISection;
class IQAItem
{
public:
    IQAItem (ISection* section);
    virtual ~IQAItem(){};
    virtual bool parseJson(const QJsonObject & json);

    QString question() const;
    ISection *section() const;
    void setSection(ISection *section);

    QString variableName() const;

protected:
    QString m_question;
    QString m_variableName;
    QSharedPointer<ISection> m_section;
};

#endif // IQAITEM_H
