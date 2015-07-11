#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWindow>
#include <QList>
#include <QString>
#include <QStringList>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QMap>
#include <QTimer>
#include <QFont>
#include <QSettings>
#include <QKeySequence>
#include <typeinfo>

#include "../model/section.h"
#include "../model/qaitemfixed.h"
#include "../model/qaitemarbitrary.h"
#include "../model/qaitemfixedimages.h"
#include "../model/test.h"

#include "dictionarydialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    enum buttonsVariants {START, SECTION_INSTRUCTION, SECTION_TEST, FINISH, INTERPRETATION};
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    QList<Section> testSections;

    QList<Section>::ConstIterator currentSection;
    QList<IQAItem*>::ConstIterator currentQA;
    QMap<QString, QStringList> sectionsAnswers;
    QTimer timer;
    int msecGone;

    Test test;

    QList<QWidget*> answerWidgets;
    QSettings settings;
    QMainWindow *m_plot;

    void updateInterface(MainWindow::buttonsVariants mode);
    void nextQuestionDisplay();

private:
    Ui::MainWindow *ui;

    int zoom;
    void clearButtons();
    void resizeWidgetText(QWidget *widget);
    void normalizeWidgets(QSize &maxSize);
    void addHotkeys();

public slots:
    void onManualProcessing(Section &section, const QStringList &sectionAnswers);

private slots:
    void nextSectionDisplay();
    void onStartClick();
    void onSectionStart();
    void onAnswerClick();
    void onTimerTick(int minRemaining);
    void onFinishClick();
    void on_actionZoomIn_triggered();
    void on_actionZoomOut_triggered();
    void onPlotReady(QCustomPlot * plot);
};

#endif // MAINWINDOW_H
