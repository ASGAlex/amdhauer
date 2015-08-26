#include "test.h"

Test::Test()
{
    m_manualProcessedScoresTotal = 0;
    plot = 0;
}

Test::~Test()
{
}

bool Test::load(QString fileName)
{
    QJsonObject root;
    try {
        root = JsonHelper::readFile(fileName);
    }
    catch (QString error) {
        qCritical() << error;
    }

    QFileInfo info(fileName);
    QDir dir = info.absoluteDir();
    m_testPath = dir.canonicalPath();

    m_name = JsonHelper::readStringValue("name", root);
    m_description = JsonHelper::readStringValue("description", root);
    m_libName = JsonHelper::readStringValue("lib", root);

    QJsonObject windowSize = JsonHelper::readObject("windowSize", root);
    m_preferredWindowSize.setWidth(JsonHelper::readIntValue("width", windowSize));
    m_preferredWindowSize.setHeight(JsonHelper::readIntValue("height", windowSize));

    m_conversionsPath = JsonHelper::readStringValue("conversionsPath", root);
    m_conversionsJson = JsonHelper::readObjectArray("conversions", root);

    QJsonObject interpretationJson = JsonHelper::readObject("interpretation", root);
    if (!interpretationJson.isEmpty()) {
        m_interpretation.load(interpretationJson);
    }

    QString sectionsPath = JsonHelper::readStringValue("sectionsPath", root);
    QStringList fileNames = JsonHelper::readStringArray("sections", root);
    loadSections(fileNames, sectionsPath);
    init();

    return true;
}

void Test::init()
{
    if (useTimer()) {
        m_timer.setInterval(1000);
        m_timer.stop();
        connect(&m_timer, SIGNAL(timeout()), SLOT(onTimerTick()));
    }
}

void Test::startTest()
{
    m_currentSection = m_sections.constBegin();
}

void Test::startSection()
{
    m_currentQA = currentSection().qaItems().constBegin();
    if (currentSection().time() > 0) {
        m_timer.start();
        m_msecGone = 0;
    }
}
QString Test::name() const
{
    return m_name;
}

QString Test::description() const
{
    return m_description;
}

bool Test::useTimer() const
{
    foreach (Section section, m_sections) {
        if (section.time() > 0)
            return true;
    }

    return false;
}
QTimer& Test::timer()
{
    return m_timer;
}

Section Test::currentSection()
{
    return *m_currentSection;
}

void Test::nextSection()
{
    if (useTimer()) {
        m_timer.stop();
    }
    m_currentSection++;
}

bool Test::sectionsFinished()
{
    return (m_currentSection == m_sections.constEnd());
}
IQAItem* Test::currentQA()
{
    return *m_currentQA;
}

void Test::nextQA()
{
    m_currentQA++;
}
bool Test::qaFinished()
{
    return (m_currentQA == currentSection().qaItems().constEnd());
}

void Test::pushAnswer(QString answer)
{
    QString variableName = currentQA()->variableName();
    if (variableName.isEmpty()) {
        QString sectionName = currentSection().name();
        QList<QString> answersList = m_sectionsAnswers.value(sectionName);

        answersList.append(answer);
        m_sectionsAnswers[sectionName] = answersList;

    } else {
        m_variables[variableName] = answer;
    }
}

void Test::pushManualProcessedAnswers(Section& section, const QList<QVariant>& answers)
{
    m_manualProcessedScoresTotal += section.processAnswers(answers);
}

int Test::calculate()
{
    QList<Section>::Iterator iSection = m_sections.begin();
    int scores = 0;

    while (iSection != m_sections.end()) {

        QStringList sectionAnswers = m_sectionsAnswers.value((*iSection).name());
        QList<QVariant> answersToProcess;

        if ((*iSection).useDictionary() && (*iSection).type() == Section::ARBITRARY) {
            emit manualProcesssing((*iSection), sectionAnswers);

        } else {
            foreach (QString s, sectionAnswers) {
                answersToProcess << QVariant(s);
            }
        }

        scores += (*iSection).processAnswers(answersToProcess);

        iSection++;
    }

    m_calculatedScoresTotal = scores;

    loadConversionTable();

    return scores;
}

QString Test::interpretationText()
{
    QString text;
    QList<Section>::Iterator iSection = m_sections.begin();
    while (iSection != m_sections.end()) {
        if (!(*iSection).interpretation().isEmpty()) {
            (*iSection).interpretation().setConversionTable(&m_conversionTable);
            text += (*iSection).interpretationText();
        }
        iSection++;
    }

    if (!m_interpretation.isEmpty()) {
        m_interpretation.setConversionTable(&m_conversionTable);
        Section fakeSection;
        fakeSection.setMachineName("total");
        m_interpretation.setSection(&fakeSection);
        QString totalText = m_interpretation.getText(totalScore());
        text += totalText;
    }

    return text;
}

bool Test::hasGraphData()
{
    foreach (Section section, m_sections) {
        if (section.interpretation().showOnGraph()) {
            return true;
        }
    }

    return false;
}

int Test::manualProcessedScores() const
{
    return m_manualProcessedScoresTotal;
}

int Test::calculatedScores() const
{
    return m_calculatedScoresTotal;
}

int Test::totalScore() const
{
    return manualProcessedScores() + calculatedScores();
}

QSize Test::preferredWindowSize() const
{
    return m_preferredWindowSize;
}

bool Test::loadConversionTable()
{
    foreach (QJsonObject object, m_conversionsJson) {

        if (this->conversionMatchConditions(object)) {
            QString fileName = JsonHelper::readStringValue("file", object);
            ConversionTable table;
            table.load(m_testPath + "/" + m_conversionsPath + "/" + fileName + ".json");
            m_conversionTable[table.type()] = table;
        }
    }

    return true;
}

bool Test::loadSections(const QStringList& fileNames, const QString& sectionsPath)
{
    bool success = true;
    foreach (QString name, fileNames) {
        QString filename = m_testPath + "/" + sectionsPath + "/" + name + ".json";

        Section section(filename);
        if (section.isReady()) {
            m_sections.append(section);
        } else {
            qCritical("Couldn't load section's settings.");
            success = false;
        }
    }
    return success;
}

bool Test::conversionMatchConditions(const QJsonObject& conversion) const
{
    QJsonObject::ConstIterator iCondition = conversion.constBegin();
    while (iCondition != conversion.constEnd()) {
        if (iCondition.key() != "file") {

            QString variable = m_variables[iCondition.key()];

            if (iCondition.value().isArray()) {
                QJsonArray array = iCondition.value().toArray();
                int from = array[0].toInt();
                int to = array[1].toInt();
                int var = variable.toInt();
                if (!(from <= var && var <= to)) {
                    return false;
                }

            } else if (iCondition.value().toString() != variable) {
                return false;
            }
        }

        iCondition++;
    }

    return true;
}

void Test::preparePlotData()
{
    if (plot == 0) {
        plot = new QCustomPlot();
        plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    }

    plot->clearItems();

    QVector<double> dataX;
    QVector<QString> labelsX;
    QVector<double> dataY;
    int index = 0;

    foreach (Section section, m_sections) {
        if (section.interpretation().showOnGraph()) {
            dataX << double(index);
            labelsX << section.interpretation().scaleShortName();

            QString name = *(section.interpretation().graphNames().begin());
            int score = section.interpretation().convertedScores()[name];
            dataY << double(score);

            QCPItemText* pointLabel = new QCPItemText(plot);
            QString pointText;
            pointText.setNum(score);
            pointLabel->setText(pointText);
            pointLabel->position->setType(QCPItemPosition::ptPlotCoords);
            pointLabel->position->setCoords(double(index), double(score));
            pointLabel->setPadding(QMargins(-1, 0, 0, 25));
            plot->addItem(pointLabel);

            index++;
        }
    }

    QCPGraph* graph = plot->addGraph();
    graph->setData(dataX, dataY);

    QCPScatterStyle scatterStyle;
    scatterStyle.setShape(QCPScatterStyle::ssCircle);
    scatterStyle.setPen(QPen(Qt::red));
    scatterStyle.setBrush(Qt::white);
    scatterStyle.setSize(10);
    graph->setScatterStyle(scatterStyle);

    graph->setLineStyle(QCPGraph::lsLine);

    plot->xAxis->setLabel("Характеристика");

    plot->xAxis->setAutoTicks(false);
    plot->xAxis->setTickVector(dataX);

    plot->xAxis->setAutoTickLabels(false);
    plot->xAxis->setTickVectorLabels(labelsX);

    plot->yAxis->setLabel("Баллы");
    plot->yAxis->setAutoSubTicks(false);
    plot->yAxis->setSubTickCount(1);
    plot->yAxis->setAutoTickStep(false);
    plot->yAxis->setTickStep(10.0);
    plot->yAxis->setRange(0.0, 140.0);
    plot->yAxis->setScaleType(QCPAxis::stLinear);

    emit plotReady(plot);
}

bool Test::saveResults(const QString fileName)
{
    QJsonObject root;

    QJsonObject variables;
    QMap<QString, QString>::ConstIterator iVar = m_variables.constBegin();
    while (iVar != m_variables.constEnd()) {
        variables.insert(iVar.key(), iVar.value());
        iVar++;
    }
    root.insert("variables", variables);

    QJsonObject answers;
    QMap<QString, QStringList>::ConstIterator iAnswer = m_sectionsAnswers.constBegin();
    QList<Section>::ConstIterator iSection = m_sections.constBegin();
    while (iAnswer != m_sectionsAnswers.constEnd() && iSection != m_sections.constEnd()) {

        QJsonArray sectionAnswers;
        QStringList::ConstIterator iSectAnswer = (*iAnswer).constBegin();

        while (iSectAnswer != (*iAnswer).constEnd()) {
            sectionAnswers.push_back(QJsonValue(*iSectAnswer));
            iSectAnswer++;
        }
        answers.insert((*iSection).machineName(), sectionAnswers);

        iAnswer++;
        iSection++;
    }
    root.insert("answers", answers);

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonDocument document(root);
    QString json(document.toJson());

    int result = file.write(json.toUtf8());
    file.close();

    return (result != -1);
}

bool Test::loadResults(const QString fileName)
{
    QJsonObject root;
    try {
        root = JsonHelper::readFile(fileName);
    }
    catch (QString error) {
        qCritical() << error;
        return false;
    }

    m_variables.empty();
    m_sectionsAnswers.empty();

    QJsonObject variables = JsonHelper::readObject("variables", root);
    QJsonObject::ConstIterator iVar = variables.constBegin();
    while (iVar != variables.constEnd()) {
        m_variables[iVar.key()] = iVar.value().toString();
        iVar++;
    }

    QJsonObject sectionAnswers = JsonHelper::readObject("answers", root);
    QList<Section>::ConstIterator iSection = m_sections.constBegin();
    while (iSection != m_sections.constEnd()) {
        QStringList sectionAnswersList = JsonHelper::readStringArray((*iSection).machineName(), sectionAnswers);
        m_sectionsAnswers[(*iSection).name()] = sectionAnswersList;
        iSection++;
    }

    return true;
}

void Test::onTimerTick()
{
    int maxTickTime = currentSection().time() * 1000;
    m_msecGone += 1000;

    int minRemaining = int(ceil((maxTickTime - m_msecGone) / 60000));
    emit timerTick(minRemaining);

    if (m_msecGone >= maxTickTime) {
        emit sectionChange();
    }
}
