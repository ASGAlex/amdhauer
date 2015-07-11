#ifndef CONVERSIONTABLE_H
#define CONVERSIONTABLE_H

#include <QString>
#include <QList>
#include <QMap>
#include <QVariant>
#include <QVariantList>

#include "../helper/jsonhelper.h"

class ConversionTable
{
public:
    ConversionTable();
    ~ConversionTable();
    void load(QString fileName);
    int convert(const QString &sectionName, const int &scores) const;
    QString type() const;
    QString name() const;

protected:
    QMap<QString, QList<QMap<QString, QVariant> > > m_data;
    QString m_type;
    QString m_name;
};

#endif // CONVERSIONTABLE_H
