#ifndef QAITEMARBITRARY_H
#define QAITEMARBITRARY_H

#include <QMap>
#include <QString>

#include "iqaitem.h"
#include "../helper/jsonhelper.h"

class QAItemArbitrary : public IQAItem
{
public:
    enum ValidatorType {NUMERIC, REGEXP};

    QAItemArbitrary(ISection* section);
    ~QAItemArbitrary();

    virtual bool parseJson(const QJsonObject & json);

    QString rightAnswer() const;
    bool isNumericAnswer() const;
    int score() const;
    QValidator *validator() const;

    ValidatorType validatorType() const;

protected:
    QString m_rightAnswer;
    bool m_numericAnswer;
    int m_score;

    QSharedPointer<QValidator> m_validator;
    QAItemArbitrary::ValidatorType m_validatorType;
};

#endif // QAITEMARBITRARY_H
