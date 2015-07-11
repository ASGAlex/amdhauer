#ifndef INTERPRETATION_H
#define INTERPRETATION_H

#include <QJsonObject>
#include <QString>
#include <QList>
#include <QStringList>
#include <QMap>
#include <QVariant>
#include "../helper/jsonhelper.h"
#include "isection.h"
#include "conversiontable.h"

class Interpretation
{
public:
    Interpretation();
    ~Interpretation();

    QString scaleShortName() const;
    QString scaleFullName() const;
    QString description() const;
    bool showOnGraph() const;
    QMap<QString, ConversionTable> *conversionTable() const;
    void setConversionTable(QMap<QString, ConversionTable> *conversionTable);
    void setSection(ISection *section);

    QString getText(const int &score);
    void load(const QJsonObject &root);
    bool isEmpty() const;

    const QMap<QString, int> &convertedScores() const;

    const QStringList &graphNames() const;

protected:
    QString m_scaleShortName;
    QString m_scaleFullName;
    QString m_description;
    QList<QMap<QString, QVariant> > m_score;
    QMap<QString, int> m_convertedScores;

    QMap<QString, ConversionTable> *m_conversionTable;
    ISection *m_section;

    QStringList m_graphNames;
    bool m_join;
    bool m_showDescription;

};

#endif // INTERPRETATION_H
