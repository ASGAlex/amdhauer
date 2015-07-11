#include "interpretation.h"

Interpretation::Interpretation()
{
    m_conversionTable = 0;
}

Interpretation::~Interpretation()
{
}
QString Interpretation::scaleShortName() const
{
    return m_scaleShortName;
}
QString Interpretation::scaleFullName() const
{
    return m_scaleFullName;
}
QString Interpretation::description() const
{
    return m_description;
}

const QStringList& Interpretation::graphNames() const
{
    return m_graphNames;
}

bool Interpretation::showOnGraph() const
{
    return !m_graphNames.isEmpty();
}

QMap<QString, ConversionTable>* Interpretation::conversionTable() const
{
    return m_conversionTable;
}

void Interpretation::setConversionTable(QMap<QString, ConversionTable>* conversionTable)
{
    m_conversionTable = conversionTable;
}

void Interpretation::setSection(ISection* section)
{
    m_section = section;
}

const QMap<QString, int>& Interpretation::convertedScores() const
{
    return m_convertedScores;
}

QString Interpretation::getText(const int& score)
{
    QString text = "<h4>" + m_scaleShortName + "<i>(" + m_scaleFullName + ")</i>:</h4><p>";

    QMap<QString, ConversionTable>::ConstIterator iConversion = m_conversionTable->constBegin();
    while (iConversion != m_conversionTable->constEnd()) {

        int convertedScore = (*iConversion).convert(m_section->machineName(), score);
        if (convertedScore != -1) {
            QString name = (*iConversion).name();
            QString sConvertedScore;
            sConvertedScore.setNum(convertedScore);
            QString description;

            QList<QMap<QString, QVariant> >::ConstIterator iMap = m_score.constBegin();
            while (iMap != m_score.constEnd()) {
                if ((*iMap)["name"].toString() == name && (*iMap)["from"].toInt() <= score && score <= (*iMap)["to"].toInt()) {
                    description += (*iMap)["description"].toString();
                }
                iMap++;
            }

            text += "<b>" + name + ": </b> " + sConvertedScore + "; " + description + "<br/>";

            m_convertedScores[name] = convertedScore;
        }
        iConversion++;
    }

    QString sScore;
    sScore.setNum(score);
    QString description;
    QList<QMap<QString, QVariant> >::ConstIterator iMap = m_score.constBegin();
    while (iMap != m_score.constEnd()) {
        if ((*iMap)["name"].toString() == "raw" && (*iMap)["from"].toInt() <= score && score <= (*iMap)["to"].toInt()) {
            description += (*iMap)["description"].toString();
        }
        iMap++;
    }
    if (description.isEmpty() && m_showDescription) {
        description = m_description;
    }
    text += "<b>\"Сырые\" баллы: </b>" + sScore + ". " + description;
    m_convertedScores["raw"] = score;
    text += "</p>";

    return text;
}

void Interpretation::load(const QJsonObject& root)
{
    m_scaleShortName = JsonHelper::readStringValue("scaleShortName", root);
    m_scaleFullName = JsonHelper::readStringValue("scaleFullName", root);
    m_showDescription = JsonHelper::readBoolValue("showDescription", root);
    m_description = JsonHelper::readStringValue("description", root);

    QJsonObject objGraph = JsonHelper::readObject("graph", root);
    m_graphNames = JsonHelper::readStringArray("names", objGraph);
    m_join = JsonHelper::readBoolValue("join", objGraph);

    QList<QJsonObject> array = JsonHelper::readObjectArray("score", root);
    foreach (QJsonObject obj, array) {
        QMap<QString, QVariant> score;

        score["name"] = JsonHelper::readStringValue("name", obj);
        score["from"] = JsonHelper::readIntValue("from", obj);
        score["to"] = JsonHelper::readIntValue("to", obj);
        score["description"] = JsonHelper::readStringValue("description", obj);

        m_score << score;
    }
}

bool Interpretation::isEmpty() const
{
    return m_scaleFullName.isEmpty() && m_scaleShortName.isEmpty() && m_description.isEmpty();
}
