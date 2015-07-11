#ifndef QAITEMFIXEDIMAGES_H
#define QAITEMFIXEDIMAGES_H

#include "qaitemfixed.h"
#include "isection.h"
#include <QList>
#include <QIcon>
#include <QString>

class QAItemFixedImages : public QAItemFixed {
public:
    QAItemFixedImages(ISection *section);
    ~QAItemFixedImages();

    virtual bool parseJson(const QJsonObject& json);
    QString getQuestionImageUri(QString sectionsPath);
    const QList<QIcon>& answerVariantsIcons();

    void setImagesPath(const QString &path);

protected:
    QString m_imagesPath;
    QList<QIcon> m_answerVariantsIcons;
};

#endif // QAITEMFIXEDIMAGES_H
