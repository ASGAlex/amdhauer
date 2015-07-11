#ifndef DICTIONARYROW_H
#define DICTIONARYROW_H

#include <QWidget>

namespace Ui {
class DictionaryRow;
}

class DictionaryRow : public QWidget
{
    Q_OBJECT

public:
    explicit DictionaryRow(QWidget* parent, const QString question, const QString answer, const int trueAnswer);
    ~DictionaryRow();

    int trueAnswer();
    QString getChangedAnswer();
    QString getQuestion();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DictionaryRow *ui;
};

#endif // DICTIONARYROW_H
