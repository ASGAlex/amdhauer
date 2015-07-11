#ifndef ISECTION_H
#define ISECTION_H

#include <QString>
#include <QList>
#include <QVariant>

#include "iqaitem.h"

class ISection
{
public:

    enum SectionTypes {FIXED, FIXED_IMAGES, ARBITRARY};

    ISection();
    ISection(QString filename);
    virtual ~ISection() {};

    virtual void load(QString filename) = 0;
    virtual bool isReady() const = 0;

    virtual QString sectionsPath() const = 0;
    virtual QString machineName() const = 0;
    virtual void setMachineName(const QString name) = 0;
    virtual SectionTypes type() const = 0;
    virtual bool useDictionary() const = 0;
    virtual QString name() const = 0;
    virtual QString startSectionText() const = 0;
    virtual int time() const = 0;
    virtual QString instruction() const = 0;
    virtual const QList<IQAItem*>& qaItems() const = 0;
    virtual int processAnswers(const QList<QVariant> &answers) = 0;
    virtual QString interpretationText() = 0;

};

#endif // ISECTION_H
