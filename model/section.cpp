#include "section.h"

Section::Section()
    :ISection()
{

}

Section::Section(QString filename)
    : ISection(filename)
{
    load(filename);
}


Section::~Section()
{
}
bool Section::isReady() const
{
    return m_ready;
}

QString Section::sectionsPath() const
{
    return m_sectionsPath;
}

QString Section::imagesPath() const
{
    return m_imagesPath;
}

QString Section::machineName() const
{
    return m_machineName;
}

void Section::setMachineName(const QString name)
{
    m_machineName = name;
}

Section::SectionTypes Section::type() const
{
    return m_type;
}

bool Section::useDictionary() const
{
    return m_useDictionary;
}

QString Section::name() const
{
    return m_name;
}

QString Section::startSectionText() const
{
    return m_startSectionText;
}

int Section::time() const
{
    return m_time;
}

QString Section::instruction() const
{
    return m_instruction;
}

const QList<IQAItem*>& Section::qaItems() const
{
    return m_qaItems;
}

int Section::processAnswers(const QList<QVariant>& answers)
{
    QList<IQAItem*>::ConstIterator iQa = qaItems().constBegin();
    QList<QVariant>::ConstIterator iAnswer = answers.constBegin();
    m_score = 0;

    while (iAnswer != answers.constEnd() && iQa != qaItems().constEnd()) {

        switch (m_type) {
        case Section::FIXED:
        case Section::FIXED_IMAGES: {
            m_score += (*iAnswer).toInt();

        } break;

        case Section::ARBITRARY: {
            QAItemArbitrary* item = dynamic_cast<QAItemArbitrary*>(*iQa);
            if (useDictionary()) {
                int calculatedScore = (*iAnswer).toInt();
                m_score += calculatedScore;

            } else if ((*iAnswer).toString().trimmed() == item->rightAnswer()) {
                m_score += item->score();
            }

        } break;
        }

        iAnswer++;
        iQa++;
    }

    return m_score;
}

QString Section::interpretationText()
{
    m_interpretation.setSection(this);
    return m_interpretation.getText(m_score);
}

Interpretation &Section::interpretation()
{
    return m_interpretation;
}
int Section::score() const
{
    return m_score;
}

IQAItem* Section::createNewQA()
{
    switch (m_type) {

    case ISection::FIXED: {
        QAItemFixed* item = new QAItemFixed(this);
            return static_cast<IQAItem*>(item);
        } break;

        case ISection::FIXED_IMAGES: {
            QAItemFixedImages* item = new QAItemFixedImages(this);
            item->setImagesPath(m_imagesPath);
            return static_cast<IQAItem*>(item);
        } break;

        case ISection::ARBITRARY: {
            QAItemArbitrary* item = new QAItemArbitrary(this);
            return static_cast<IQAItem*>(item);
        } break;

    }

    throw QString("Invalid data type");
}

void Section::load(QString fileName)
{
    QJsonObject root;
    try {
        root = JsonHelper::readFile(fileName);
    } catch (QString error){
        qCritical() << error;
    }

    QFileInfo info(fileName);
    m_machineName = info.baseName();
    QDir dir = info.absoluteDir();
    m_sectionsPath = dir.canonicalPath();

    QString type = JsonHelper::readStringValue("type", root);
    if (type != "fixed" && type != "fixed_images" && type != "arbitrary") {
        throw QString("Unknown JSON file type: "+ type +" Must be either \"fixed\" or \"arbitrary\".");

    } else {
        if (type == "fixed") {
            m_type = Section::FIXED;

        } else if (type == "fixed_images") {
            m_type = Section::FIXED_IMAGES;

        } else if (type == "arbitrary") {
            m_type = Section::ARBITRARY;
        }
    }
    m_useDictionary = JsonHelper::readBoolValue("useDictionary", root);

    m_name = JsonHelper::readStringValue("name", root);
    m_startSectionText = JsonHelper::readStringValue("startSectionText", root);
    m_instruction = JsonHelper::readStringValue("instruction", root);
    m_time = JsonHelper::readIntValue("time", root);
    m_imagesPath = JsonHelper::readStringValue("imagesPath",root);

    QJsonObject interpretation = JsonHelper::readObject("interpretation",root);
    if(!interpretation.isEmpty()){
        m_interpretation.load(interpretation);
    }

    QList<QJsonObject> qa = JsonHelper::readObjectArray("qa", root);
    QList<QJsonObject>::ConstIterator i = qa.constBegin();
    while (i != qa.constEnd()) {
        try {
            IQAItem* item = createNewQA();
            item->parseJson(*i);
            m_qaItems.append(item);
        }
        catch (QString exception) {
            qCritical() << exception;
        }
        i++;
    }

    m_ready = true;
}
