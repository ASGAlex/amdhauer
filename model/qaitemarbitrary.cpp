#include "qaitemarbitrary.h"

QAItemArbitrary::QAItemArbitrary(ISection *section)
    :IQAItem(section)
{

}

QAItemArbitrary::~QAItemArbitrary()
{

}

bool QAItemArbitrary::parseJson(const QJsonObject &json)
{
    IQAItem::parseJson(json);

    m_rightAnswer = JsonHelper::readStringValue("rightAnswer", json);
    m_numericAnswer = JsonHelper::readBoolValue("numericAnswer", json);
    m_score = JsonHelper::readIntValue("score", json);

    QJsonObject validator = JsonHelper::readObject("validator", json);
    if(!validator.isEmpty()) {
        QString type = JsonHelper::readStringValue("type", validator);

        if(type == "int") {
            int top = JsonHelper::readIntValue("top", validator);
            int bottom = JsonHelper::readIntValue("bottom", validator);
            QIntValidator *validator = new QIntValidator(bottom, top);

            m_validator.reset(validator);
            m_validatorType = QAItemArbitrary::NUMERIC;

        } else if(type == "regexp") {
            QString regexp = JsonHelper::readStringValue("regexp", validator);
            QRegExpValidator *validator = new QRegExpValidator(QRegExp(regexp));

            m_validator.reset(validator);
            m_validatorType = QAItemArbitrary::REGEXP;
        }
    }

    return true;
}

QString QAItemArbitrary::rightAnswer() const
{
    return m_rightAnswer;
}

bool QAItemArbitrary::isNumericAnswer() const
{
    return m_numericAnswer;
}

int QAItemArbitrary::score() const
{
    return m_score;
}

QValidator *QAItemArbitrary::validator() const
{
    if(m_validator.isNull()){
        throw QString("Validator is empty");
    } else {
        return m_validator.data();
    }
}

QAItemArbitrary::ValidatorType QAItemArbitrary::validatorType() const
{
    return m_validatorType;
}
