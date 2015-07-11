#ifndef SECTION_H
#define SECTION_H

#include <QString>
#include <QList>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QDebug>
#include <QVariant>
#include <QFileInfo>
#include <QDir>

#include "../helper/jsonhelper.h"
#include "qaitemfixed.h"
#include "qaitemarbitrary.h"
#include "qaitemfixedimages.h"
#include "interpretation.h"

class Section :public ISection
{
public:
    Section();
    Section(QString filename);
    ~Section();

    void load(QString machineName);
    bool isReady() const;
    QString sectionsPath() const;
    QString imagesPath() const;
    virtual QString machineName() const;
    virtual void setMachineName(const QString name);
    SectionTypes type() const;
    bool useDictionary() const;
    QString name() const;
    QString startSectionText() const;
    int time() const;
    QString instruction() const;
    const QList<IQAItem*>& qaItems() const;
    int processAnswers(const QList<QVariant> &answers);
    QString interpretationText();
    Interpretation& interpretation();
    int score() const;

protected:
    bool m_ready;
    QString m_machineName;
    QString m_sectionsPath;
    SectionTypes m_type;
    bool m_useDictionary;
    QString m_name;
    int m_time;
    QString m_instruction;
    QString m_imagesPath;
    QString m_startSectionText;
    QList<IQAItem*> m_qaItems;

    int m_score;
    Interpretation m_interpretation;

    IQAItem *createNewQA();

};

#endif // SECTION_H
