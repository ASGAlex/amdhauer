#include "dictionary.h"

Dictionary::Dictionary()
{
}

Dictionary::Dictionary(const QString& fileName)
{
    this->load(fileName);
}

Dictionary::~Dictionary()
{
}

bool Dictionary::load(const QString& fileName)
{
    QJsonObject root;
    try {
        root = JsonHelper::readFile(fileName);
    }
    catch (QString error) {
        qCritical() << error;
    }

    QJsonObject::ConstIterator i = root.constBegin();
    while (i != root.constEnd()) {

        QList<QJsonObject> variants = JsonHelper::readObjectArray(i.key(), root);
        QSet<RatedAnswer> variantsFiltered;
        foreach (QJsonObject ratedAnswer, variants) {
            int score = JsonHelper::readIntValue("score", ratedAnswer);
            QString answer = JsonHelper::readStringValue("answer", ratedAnswer);
            RatedAnswer ra(score, answer);
            variantsFiltered << ra;
        }

        this->m_dictionary.insert(i.key(), variantsFiltered);

        i++;
    }

    return true;
}

bool Dictionary::save(const QString& fileName)
{
    QJsonObject root;
    QMap<QString, QString> qtbugReplacement;
    int qtbugIndex = 0;

    QMap<QString, QSet<RatedAnswer> >::ConstIterator i = this->m_dictionary.constBegin();

    while (i != this->m_dictionary.constEnd()) {

        QSet<RatedAnswer>::ConstIterator iSet = (*i).constBegin();
        QJsonArray array;

        while (iSet != (*i).constEnd()) {
            array << (*iSet).toJsonObject();
            iSet++;
        }

        QString qtbugKey = "key"+QString("%1").arg(qtbugIndex, 5, 10, QChar('0'));
        qtbugReplacement[qtbugKey] = i.key();
        root.insert(qtbugKey, array);

        qtbugIndex++;
        i++;
    }

    qDebug() << "Save dict to: " << fileName;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonDocument document(root);
    QString json(document.toJson());

    QMap<QString, QString>::ConstIterator iQtbug = qtbugReplacement.constBegin();
    while (iQtbug != qtbugReplacement.constEnd()) {
        QString before = iQtbug.key();
        QString after = iQtbug.value();
        json = json.replace(before, after);

        iQtbug++;
    }

    int result = file.write(json.toUtf8());
    file.close();

    return (result != -1);
}

void Dictionary::update(const QString question, const QString answer, const int trueAnswer)
{
    RatedAnswer ra(trueAnswer, answer);
    if (this->m_dictionary.contains(question) && this->m_dictionary[question].contains(ra) && !trueAnswer) {
        this->m_dictionary[question].remove(ra);

    } else if (trueAnswer) {
        this->m_dictionary[question] << ra;
    }
}
const QSet<RatedAnswer>& Dictionary::rightAnswers(const QString& question) const
{
    return m_dictionary.value(question);
}
