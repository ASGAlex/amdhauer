#include "jsonhelper.h"

JsonHelper::JsonHelper()
{

}

JsonHelper::~JsonHelper()
{

}

QJsonObject JsonHelper::readFile(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        throw QString("JSON: can't open file ")+fileName;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError* error = new QJsonParseError();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(data, error));
    if (error->error != QJsonParseError::NoError) {
        throw QString("JSON file "+fileName+" failed to parse:"+error->errorString());
    }

    return jsonDoc.object();
}

QString JsonHelper::readStringValue(const QString name, const QJsonObject &root)
{
    QJsonValue value = root.value(name);
    if(value != QJsonValue::Undefined){
        return value.toString();
    }

    return QString("");
}

int JsonHelper::readIntValue(const QString name, const QJsonObject &root)
{
    QJsonValue value = root.value(name);
    if(value != QJsonValue::Undefined){
        return value.toInt();
    }

    return 0;
}

bool JsonHelper::readBoolValue(const QString name, const QJsonObject &root)
{
    QJsonValue value = root.value(name);
    if(value != QJsonValue::Undefined){
        return value.toBool();
    }

    return false;
}

QJsonObject JsonHelper::readObject(const QString name, const QJsonObject &root)
{
    QJsonValue value = root.value(name);
    if(value != QJsonValue::Undefined){
        return value.toObject();
    }

    return QJsonObject();
}

QStringList JsonHelper::readStringArray(const QString name, const QJsonObject &root)
{
    QStringList result;

    QJsonValue array = root.value(name);
    if(array == QJsonValue::Undefined || !array.isArray()){
        qCritical()<<"JSON property \""<< name <<"\" does not exist. Array of strings expected";
        return result;
    }

    QJsonArray jsonArray = array.toArray();
    QJsonArray::ConstIterator i = jsonArray.constBegin();
    while(i != jsonArray.constEnd()) {
        result.append((*i).toString());
        i++;
    }

    return result;
}

QList<int> JsonHelper::readIntArray(const QString name, const QJsonObject &root)
{
    QList<int> result;

    QJsonValue array = root.value(name);
    if(array == QJsonValue::Undefined || !array.isArray()){
        qCritical()<<"JSON property \""<< name <<"\" does not exist. Array of int expected";
        return result;
    }

    QJsonArray jsonArray = array.toArray();
    QJsonArray::ConstIterator i = jsonArray.constBegin();
    while(i != jsonArray.constEnd()) {
        result.append((*i).toInt());
        i++;
    }

    return result;
}

QList<QJsonObject> JsonHelper::readObjectArray(const QString name, const QJsonObject &root)
{
    QList<QJsonObject> result;

    QJsonValue array = root.value(name);
    if(array == QJsonValue::Undefined || !array.isArray()){
        qCritical()<<"JSON property \""<< name <<"\" does not exist. Array of objects expected";
        return result;
    }

    QJsonArray jsonArray = array.toArray();
    QJsonArray::ConstIterator i = jsonArray.constBegin();
    while(i != jsonArray.constEnd()) {
        if(!(*i).isObject()) continue;

        result.append((*i).toObject());
        i++;
    }

    return result;
}
