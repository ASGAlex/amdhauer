#include "dictionarydialog.h"
#include "ui_dictionarydialog.h"

DictionaryDialog::DictionaryDialog(QWidget* parent, const Section& section, const QStringList& answers)
    : QDialog(parent)
    , ui(new Ui::DictionaryDialog)
{
    this->hasFalseAnswers = false;

    ui->setupUi(this);
    QPushButton* apply = ui->buttonBox->button(QDialogButtonBox::Apply);
    connect(apply, SIGNAL(clicked()), SLOT(onDialogAccepted()));

    this->ui->textBrowser->setHtml(section.instruction());

    QFileInfo info(section.machineName());

    this->dictionaryFileName = info.baseName() + ".dict.json";
    this->dictionary.load(this->dictionaryFileName);

    QList<IQAItem*>::ConstIterator iQa = section.qaItems().constBegin();
    QStringList::ConstIterator iAnswer = answers.constBegin();

    while (iAnswer != answers.constEnd() && iQa != section.qaItems().constEnd()) {

        QSet<RatedAnswer> rightAnswers = this->dictionary.rightAnswers((*iQa)->question());

        RatedAnswer ra((*iAnswer).toLower());
        QSet<RatedAnswer>::ConstIterator iRatedAnswer = rightAnswers.find(ra);
        int trueAnswer = 0;
        if (iRatedAnswer == rightAnswers.constEnd()) {
            this->hasFalseAnswers = true;
        } else {
            trueAnswer = (*iRatedAnswer).score();
        }

        DictionaryRow* row = new DictionaryRow(this, (*iQa)->question(), *iAnswer, trueAnswer);
        this->rows.append(row);
        this->ui->rowsLayout->addWidget(row);

        iAnswer++;
        iQa++;
    }

    this->answers = answers;
}

DictionaryDialog::~DictionaryDialog()
{
    delete ui;
}

const QMap<QString, int>& DictionaryDialog::checkedAnswers()
{
    return m_checkedAnswers;
}

void DictionaryDialog::changeEvent(QEvent* e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DictionaryDialog::onDialogAccepted()
{
    try {
        QList<DictionaryRow*>::ConstIterator i = this->rows.constBegin();
        while (i != this->rows.constEnd()) {

            this->m_checkedAnswers[(*i)->getChangedAnswer()] = (*i)->trueAnswer();
            this->dictionary.update((*i)->getQuestion(), (*i)->getChangedAnswer(), (*i)->trueAnswer());

            i++;
        }
        this->dictionary.save(this->dictionaryFileName);
        this->accept();
    }
    catch (QString exception) {
        QMessageBox::information(this, "Не все поля заполнены", exception);
    }
}
