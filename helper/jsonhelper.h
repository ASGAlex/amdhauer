#ifndef JSONHELPER_H
#define JSONHELPER_H

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QByteArray>
#include <QList>
#include <QFile>

class JsonHelper
{
public:
    JsonHelper();
    ~JsonHelper();

    static QJsonObject readFile(const QString &fileName);
    static QString readStringValue(const QString name, const QJsonObject &root);
    static int readIntValue(const QString name, const QJsonObject &root);
    static bool readBoolValue(const QString name, const QJsonObject &root);
    static QJsonObject readObject(const QString name, const QJsonObject &root);
    static QStringList readStringArray(const QString name, const QJsonObject &root);
    static QList<int> readIntArray(const QString name, const QJsonObject &root);
    static QList<QJsonObject> readObjectArray(const QString name, const QJsonObject &root);
};

#endif // JSONHELPER_H
