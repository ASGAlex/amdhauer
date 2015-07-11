#ifndef QAITEM_H
#define QAITEM_H

#include <QString>
#include <QJsonObject>
#include <QStringList>
#include <QList>

#include "../helper/jsonhelper.h"
#include "iqaitem.h"

class QAItemFixed : public IQAItem {
public:
    QAItemFixed(ISection *section);
    ~QAItemFixed();

    virtual bool parseJson(const QJsonObject& json);
    const QStringList& answerVariants() const;
    const QList<int>& answerScore() const;

protected:
    QStringList m_answerVariants;
    QList<int> m_answerScore;
};

#endif // QAITEM_H
