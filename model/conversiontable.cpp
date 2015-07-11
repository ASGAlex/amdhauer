#include "conversiontable.h"

ConversionTable::ConversionTable()
{
    m_type = "default";
}

ConversionTable::~ConversionTable()
{
}

void ConversionTable::load(QString fileName)
{
    QJsonObject root;
    try {
        root = JsonHelper::readFile(fileName);
    }
    catch (QString error) {
        qCritical() << error;
    }

    QJsonObject::ConstIterator iRoot = root.constBegin();
    while (iRoot != root.constEnd()) {
        if ((*iRoot).isArray()) {

            QString sectionName = iRoot.key();
            QList<QJsonObject> conversions = JsonHelper::readObjectArray(sectionName, root);
            QList<QMap<QString, QVariant> > rulesList;

            foreach (QJsonObject jsonRule, conversions) {

                QMap<QString, QVariant> rule;
                QJsonValue raw = jsonRule.value("raw");

                if (raw.isArray()) {
                    rule["raw"] = raw.toVariant();
                } else {
                    rule["raw"] = raw.toInt();
                }

                int scoreNew = jsonRule.value("new").toInt();
                rule["new"] = scoreNew;

                rulesList << rule;
            }
            m_data[sectionName] = rulesList;

        } else if ((*iRoot).isString()) {
            if (iRoot.key() == "type") {
                m_type = (*iRoot).toString();
            } else if (iRoot.key() == "name") {
                m_name = (*iRoot).toString();
            }
        }
        iRoot++;
    }
}

int ConversionTable::convert(const QString& sectionName, const int& scores) const
{
    if (sectionName == "condition")
        return -1;

    QList<QMap<QString, QVariant> >::ConstIterator iRule = m_data[sectionName].constBegin();

    while (iRule != m_data[sectionName].constEnd()) {

        bool successConversion = false;
        int i_raw = (*iRule)["raw"].toInt(&successConversion);
        if (successConversion) {
            if (scores == i_raw) {
                return (*iRule)["new"].toInt();
            }

        } else {
            QVariantList jsonArray = (*iRule)["raw"].toList();
            int from = jsonArray[0].toInt();
            int to = jsonArray[1].toInt();

            if (from <= scores && scores <= to) {
                return (*iRule)["new"].toInt();
            }
        }

        iRule++;
    }

    return -1;
}

QString ConversionTable::type() const
{
    return m_type;
}

QString ConversionTable::name() const
{
    return m_name;
}
