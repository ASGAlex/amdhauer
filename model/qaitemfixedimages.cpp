#include "qaitemfixedimages.h"

QAItemFixedImages::QAItemFixedImages(ISection *section)
    :QAItemFixed(section)
{
}

QAItemFixedImages::~QAItemFixedImages()
{
}

bool QAItemFixedImages::parseJson(const QJsonObject& json)
{
    QAItemFixed::parseJson(json);

    QStringList::ConstIterator i = answerVariants().constBegin();
    while (i != answerVariants().constEnd()) {
        QString answerFilename = section()->sectionsPath() + "/" +m_imagesPath + "/" + (*i);
        QIcon icon(answerFilename);
        m_answerVariantsIcons.append(icon);
        i++;
    }

    return true;
}

/**
 * FIXME: section() call produce segfault
 *
 * @brief QAItemFixedImages::getQuestionImageUri
 * @param sectionsPath
 * @return QString
 */
QString QAItemFixedImages::getQuestionImageUri(QString sectionsPath)
{
    return sectionsPath + "/"+m_imagesPath + "/" + m_question;
}

const QList<QIcon>& QAItemFixedImages::answerVariantsIcons()
{
    return m_answerVariantsIcons;
}

void QAItemFixedImages::setImagesPath(const QString &path)
{
    m_imagesPath = path;
}
