#ifndef TEST_H
#define TEST_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include <QTimer>
#include <QMainWindow>
#include <QDir>
#include <QSize>
#include <QVector>
#include <math.h>

#include "section.h"
#include "qaitemfixed.h"
#include "qaitemarbitrary.h"
#include "qaitemfixedimages.h"
#include "conversiontable.h"
#include "../helper/jsonhelper.h"
#include "../lib/qcustomplot.h"

class Test : public QObject
{
    Q_OBJECT

public:
    Test();
    ~Test();

    bool load(QString fileName);
    void init();
    void startTest();
    void startSection();

    QString name() const;
    QString description() const;
    bool useTimer() const;
    QTimer &timer();

    Section currentSection();
    void nextSection();
    bool sectionsFinished();

    IQAItem* currentQA();
    void nextQA();
    bool qaFinished();

    void pushAnswer(QString answer);
    void pushManualProcessedAnswers(Section &section, const QList<QVariant> &answers);

    int calculate();
    QString interpretationText();
    bool hasGraphData();

    int manualProcessedScores() const;
    int calculatedScores() const;
    int totalScore() const;

    QSize preferredWindowSize() const;

    bool loadConversionTable();

protected:
    QString m_name;
    QString m_description;
    QTimer m_timer;

    QString m_testPath;
    QList<Section> m_sections;
    QList<Section>::ConstIterator m_currentSection;
    QList<IQAItem*>::ConstIterator m_currentQA;
    QMap<QString, QStringList> m_sectionsAnswers;
    int m_msecGone;

    int m_calculatedScoresTotal;
    int m_manualProcessedScoresTotal;

    QSize m_preferredWindowSize;
    QString m_libName;

    QMap<QString, QString> m_variables;
    QString m_conversionsPath;
    QList<QJsonObject> m_conversionsJson;
    QMap<QString, ConversionTable> m_conversionTable;

    Interpretation m_interpretation;

    QCustomPlot *plot;

    bool loadSections(const QStringList &fileNames, const QString &sectionsPath);
    bool conversionMatchConditions(const QJsonObject &conversion) const;

public slots:
    void preparePlotData();
    bool saveResults(const QString fileName);
    bool loadResults(const QString fileName);

protected slots:
    void onTimerTick();

signals:
    void timerTick(int minRemaining);
    void sectionChange();
    void manualProcesssing(Section &section, const QStringList &sectionAnswers);
    void plotReady(QCustomPlot * plot);
};

#endif // TEST_H
