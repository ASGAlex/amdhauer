#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settings("OpenPsy", "Test Runner Window")
{
    ui->setupUi(this);

    this->zoom = this->settings.value("zoom", 0).toInt();
    if (this->zoom > 0) {
        for (int i = 0; i < this->zoom; i++) {
            this->ui->textBrowser->zoomIn();
        }
    }

    this->test.load("data/test.json");
    if (this->test.useTimer()) {
        connect(&this->test, SIGNAL(timerTick(int)), SLOT(onTimerTick(int)));
    }

    connect(&this->test, SIGNAL(manualProcesssing(Section&, const QStringList&)), this, SLOT(onManualProcessing(Section&, const QStringList&)));
    connect(&this->test, SIGNAL(sectionChange()), SLOT(nextSectionDisplay()));

    this->setWindowTitle(this->test.name());
    this->setMinimumSize(this->test.preferredWindowSize());

    this->updateInterface(MainWindow::START);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::changeEvent(QEvent* e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
void MainWindow::updateInterface(MainWindow::buttonsVariants mode)
{
    this->clearButtons();

    switch (mode) {
    case MainWindow::START: {
        QPushButton* start = new QPushButton();
        start->setText("Начать тестировнаие");
        connect(start, SIGNAL(clicked()), SLOT(onStartClick()));
        this->answerWidgets.append(start);
        this->ui->buttonsLayout->addWidget(start);

        this->ui->textBrowser->setHtml(this->test.description());

    } break;

    case MainWindow::SECTION_INSTRUCTION: {
        if (this->test.currentSection().type() == Section::FIXED_IMAGES) {
            QStringList paths;
            paths << this->test.currentSection().sectionsPath();
            paths << this->test.currentSection().sectionsPath() + "/" + this->test.currentSection().imagesPath();
            this->ui->textBrowser->setSearchPaths(paths);
        }
        this->ui->textBrowser->setHtml(this->test.currentSection().instruction());
        QPushButton* startSection = new QPushButton();
        QString buttonText = (this->test.currentSection().startSectionText().isEmpty() ? "Начать раздел" : this->test.currentSection().startSectionText());
        startSection->setText(buttonText);
        connect(startSection, SIGNAL(clicked()), SLOT(onSectionStart()));
        this->answerWidgets.append(startSection);
        this->ui->buttonsLayout->addWidget(startSection);

    } break;

    case MainWindow::SECTION_TEST: {
        this->test.startSection();
        this->nextQuestionDisplay();

    } break;

    case MainWindow::FINISH: {
        QPushButton* finish = new QPushButton();
        finish->setText("Перейти к обработке результатов");
        connect(finish, SIGNAL(clicked()), SLOT(onFinishClick()));
        this->answerWidgets.append(finish);
        this->ui->buttonsLayout->addWidget(finish);
        this->ui->textBrowser->setHtml("Все задания методики вы выполнили. Большое спасибо!");

    } break;

    case MainWindow::INTERPRETATION: {
        if (this->test.hasGraphData()) {
            QPushButton* graph = new QPushButton();
            graph->setText("Показать график");
            connect(graph, SIGNAL(clicked()), &test, SLOT(preparePlotData()));
            connect(&test, SIGNAL(plotReady(QCustomPlot*)), SLOT(onPlotReady(QCustomPlot*)));
            this->answerWidgets.append(graph);
            this->ui->buttonsLayout->addWidget(graph);

            m_plot = new QMainWindow(this);
        }

    } break;
    }

    foreach (QWidget* widget, this->answerWidgets) {
        this->resizeWidgetText(widget);
    }
}

void MainWindow::nextSectionDisplay()
{
    this->test.nextSection();
    if (this->test.sectionsFinished()) {
        this->updateInterface(MainWindow::FINISH);
    } else {
        this->updateInterface(MainWindow::SECTION_INSTRUCTION);
    }
}

void MainWindow::nextQuestionDisplay()
{
    if (!this->test.qaFinished()) {

        this->clearButtons();

        if (this->test.currentSection().type() != Section::FIXED_IMAGES) {
            QString questionText = this->test.currentQA()->question();
            if (questionText.isEmpty()) {
                questionText = this->test.currentSection().instruction();
            }
            this->ui->textBrowser->setHtml(questionText);
        }

        if (this->test.currentSection().type() == Section::FIXED || this->test.currentSection().type() == Section::FIXED_IMAGES) {

            QAItemFixed* curQA = static_cast<QAItemFixed*>(this->test.currentQA());

            QStringList::ConstIterator iString = curQA->answerVariants().constBegin();
            QList<int>::ConstIterator iScore = curQA->answerScore().constBegin();

            QAItemFixedImages* curQAImages;
            QList<QIcon>::ConstIterator iIcon;

            if (this->test.currentSection().type() == Section::FIXED_IMAGES) {
                curQAImages = static_cast<QAItemFixedImages*>(this->test.currentQA());
                iIcon = curQAImages->answerVariantsIcons().constBegin();
            }

            QSize maxButtonSize;
            while (iString != curQA->answerVariants().constEnd() && iScore != curQA->answerScore().constEnd()) {

                QPushButton* btAnswer = new QPushButton(this);

                if (this->test.currentSection().type() == Section::FIXED) {
                    btAnswer->setText(*iString);

                } else if (this->test.currentSection().type() == Section::FIXED_IMAGES) {
                    btAnswer->setIcon((*iIcon));

                    QSize maxSize(200, 200);
                    QSize actualSize = (*iIcon).actualSize(maxSize, QIcon::Normal, QIcon::On);
                    btAnswer->setMinimumSize(actualSize);
                    btAnswer->setIconSize(actualSize);

                    QString uri = curQAImages->getQuestionImageUri(test.currentSection().sectionsPath());
                    this->ui->textBrowser->setHtml("<html><body><img src=\"" + uri + "\"></body></html>");

                    if (maxButtonSize.width() < actualSize.width()) {
                        maxButtonSize.setWidth(actualSize.width());
                    }
                    if (maxButtonSize.height() < actualSize.height()) {
                        maxButtonSize.setHeight(actualSize.height());
                    }

                    iIcon++;
                }

                QString num;
                num.setNum(*iScore);
                btAnswer->setObjectName(num);
                connect(btAnswer, SIGNAL(clicked()), SLOT(onAnswerClick()));

                this->answerWidgets.append(btAnswer);
                this->ui->buttonsLayout->addWidget(btAnswer);
                this->resizeWidgetText(btAnswer);

                iString++;
                iScore++;
            }

            this->addHotkeys();
            if (this->test.currentSection().type() == Section::FIXED_IMAGES) {
                this->normalizeWidgets(maxButtonSize);
            }

        } else if (this->test.currentSection().type() == Section::ARBITRARY) {
            QAItemArbitrary* curQA = dynamic_cast<QAItemArbitrary*>(this->test.currentQA());

            QPushButton* btAnswer = new QPushButton(this);
            QLineEdit* lineEdit = new QLineEdit(btAnswer);

            btAnswer->setText("Далее");

            btAnswer->setObjectName("lineEditButton");
            lineEdit->setObjectName("lineEditAnswer");

            try {
                QValidator* validator = curQA->validator();
                switch (curQA->validatorType()) {

                case QAItemArbitrary::NUMERIC: {
                    QIntValidator* intValidator = static_cast<QIntValidator*>(validator);
                    lineEdit->setValidator(intValidator);
                } break;

                case QAItemArbitrary::REGEXP: {
                    QRegExpValidator* regexpValidator = static_cast<QRegExpValidator*>(validator);
                    lineEdit->setValidator(regexpValidator);
                } break;
                }
            }
            catch (QString message) {
                qWarning() << "No validator specified for question \"" << curQA->question() << "\". Creating default.";

                if (curQA->isNumericAnswer()) {
                    lineEdit->setValidator(new QIntValidator());
                } else {
                    lineEdit->setValidator(new QRegExpValidator(QRegExp(".+")));
                }
            }

            this->answerWidgets.append(btAnswer);
            this->answerWidgets.append(lineEdit);

            this->ui->buttonsLayout->addWidget(lineEdit);
            this->ui->buttonsLayout->addWidget(btAnswer);

            this->resizeWidgetText(lineEdit);
            this->resizeWidgetText(btAnswer);

            connect(btAnswer, SIGNAL(clicked()), SLOT(onAnswerClick()));
            connect(lineEdit, SIGNAL(returnPressed()), btAnswer, SLOT(click()));

            lineEdit->setFocus(Qt::MouseFocusReason);
        }

    } else {
        this->nextSectionDisplay();
    }
}

void MainWindow::clearButtons()
{
    if (!this->answerWidgets.isEmpty()) {
        QList<QWidget*>::ConstIterator i = this->answerWidgets.constBegin();
        while (i != answerWidgets.constEnd()) {
            (*i)->disconnect();
            (*i)->setParent(NULL);
            (*i)->deleteLater();
            i++;
        }
        this->answerWidgets.clear();
    }
}

void MainWindow::onStartClick()
{
    this->test.startTest();
    this->updateInterface(MainWindow::SECTION_INSTRUCTION);
}

void MainWindow::onSectionStart()
{
    this->updateInterface(MainWindow::SECTION_TEST);
}

void MainWindow::onAnswerClick()
{
    QString answer = sender()->objectName();
    if (answer == "lineEditButton") {
        QLineEdit* lineEdit = this->findChild<QLineEdit*>("lineEditAnswer");
        answer = lineEdit->text();
        int pos = 0;
        if (lineEdit->validator() != 0 && lineEdit->validator()->validate(answer, pos) != QValidator::Acceptable) {
            return;
        }
    }

    this->test.pushAnswer(answer);

    this->test.nextQA();
    this->nextQuestionDisplay();
}

void MainWindow::onTimerTick(int minRemaining)
{
    QString text;
    if (minRemaining < 1) {
        text = "Осталось меньше минуты";
    } else {
        text.setNum(minRemaining);
        text = "Осталось минут: " + text;
    }
    this->ui->statusBar->showMessage(text, 1000);
}

void MainWindow::onFinishClick()
{
    this->test.calculate();
    this->ui->textBrowser->setHtml(this->test.interpretationText());
    this->updateInterface(MainWindow::INTERPRETATION);
}

void MainWindow::onManualProcessing(Section& section, const QStringList& sectionAnswers)
{
    DictionaryDialog dictionaryDialog(this->ui->centralWidget, section, sectionAnswers);
    if (dictionaryDialog.hasFalseAnswers) {
        dictionaryDialog.exec();
    } else {
        dictionaryDialog.onDialogAccepted();
    }

    QList<QVariant> answersToProcess;
    QMap<QString, int>::ConstIterator iCheckedAnswers = dictionaryDialog.checkedAnswers().constBegin();
    while (iCheckedAnswers != dictionaryDialog.checkedAnswers().constEnd()) {
        answersToProcess << QVariant((*iCheckedAnswers));
        iCheckedAnswers++;
    }

    this->test.pushManualProcessedAnswers(section, answersToProcess);
}

void MainWindow::on_actionZoomIn_triggered()
{
    this->zoom++;
    this->ui->textBrowser->zoomIn();
    foreach (QWidget* w, this->answerWidgets) {
        this->resizeWidgetText(w);
    }
}

void MainWindow::on_actionZoomOut_triggered()
{
    this->zoom--;
    this->ui->textBrowser->zoomOut();
    foreach (QWidget* w, this->answerWidgets) {
        this->resizeWidgetText(w);
    }
}

void MainWindow::onPlotReady(QCustomPlot* plot)
{
    if (!m_plot->isVisible()) {
        m_plot->setCentralWidget(plot);
        m_plot->show();
    }
}

void MainWindow::resizeWidgetText(QWidget* widget)
{
    QString defaultFamily = widget->font().family();
    qreal defaultSize = 10;
    QFont font(defaultFamily, defaultSize + this->zoom);
    widget->setFont(font);

    this->settings.setValue("zoom", this->zoom);
}

void MainWindow::normalizeWidgets(QSize& maxSize)
{
    QList<QWidget*>::Iterator i = this->answerWidgets.begin();
    while (i != this->answerWidgets.end()) {
        (*i)->setMinimumSize(maxSize);
        i++;
    }

    int padding = 10;
    int minWidth = this->answerWidgets.count() * (maxSize.width() + padding);
    this->resize(minWidth, this->height());
}

void MainWindow::addHotkeys()
{
    QList<QWidget*>::Iterator i = this->answerWidgets.begin();
    int index = 1;
    while (i != this->answerWidgets.end()) {
        QPushButton *pb = dynamic_cast<QPushButton*>((*i));
        if(pb){
            QString sKey;
            sKey.setNum(index);
            QKeySequence ks(sKey);
            pb->setShortcut(ks);
            index++;
        }
        i++;
    }
}
