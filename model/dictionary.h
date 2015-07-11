#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <QString>
#include <QStringList>
#include <QSet>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QMap>
#include <QDebug>
#include "../helper/jsonhelper.h"
#include "ratedanswer.h"



class Dictionary {
public:
    Dictionary();
    Dictionary(const QString& fileName);
    ~Dictionary();

    bool load(const QString& fileName);
    bool save(const QString& fileName);
    void update(const QString question, const QString answer, const int trueAnswer);
    const QSet<RatedAnswer>& rightAnswers(const QString& question) const;

protected:
    QMap<QString, QSet<RatedAnswer> > m_dictionary;
};

#endif // DICTIONARY_H
