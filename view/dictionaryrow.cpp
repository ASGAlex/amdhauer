#include "dictionaryrow.h"
#include "ui_dictionaryrow.h"

DictionaryRow::DictionaryRow(QWidget *parent, const QString question, const QString answer, const int trueAnswer) :
    QWidget(parent),
    ui(new Ui::DictionaryRow)
{
    ui->setupUi(this);
    this->ui->question->setText(question);
    this->ui->answer->setText(answer);

    if(trueAnswer == 2){
        this->ui->trueAnswer->setChecked(true);
    } else if(trueAnswer == 1) {
        this->ui->semiTrueAnswer->setChecked(true);
    } else if(!trueAnswer) {
        this->ui->falseAnswer->setChecked(true);
    }
}

DictionaryRow::~DictionaryRow()
{
    delete ui;
}

int DictionaryRow::trueAnswer()
{
    if(this->ui->trueAnswer->isChecked()){
        return 2;
    } else if(ui->semiTrueAnswer->isChecked()){
        return 1;
    } else if(ui->falseAnswer->isChecked()){
        return 0;
    } else {
        QString exception = "Не указана правильность ответа на вопрос \""+ui->question->text()+"\"";
        throw exception;
    }
}

QString DictionaryRow::getChangedAnswer()
{
    return this->ui->answer->text().toLower();
}

QString DictionaryRow::getQuestion()
{
    return this->ui->question->text();
}

void DictionaryRow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
