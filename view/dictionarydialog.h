#ifndef DICTIONARYDIALOG_H
#define DICTIONARYDIALOG_H

#include <QDialog>
#include <QList>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QFileInfo>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QMessageBox>
#include "../model/dictionary.h"
#include "../model/section.h"
#include "../model/iqaitem.h"
#include "../model/dictionary.h"

#include "dictionaryrow.h"

namespace Ui {
class DictionaryDialog;
}

class DictionaryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DictionaryDialog(QWidget *parent, const Section &section, const QStringList &answers);
    ~DictionaryDialog();

    bool hasFalseAnswers;
    const QMap<QString, int>& checkedAnswers();

protected:
    void changeEvent(QEvent *e);

    QMap<QString, int> m_checkedAnswers;
    QStringList answers;
    QList<DictionaryRow*> rows;
    Dictionary dictionary;
    QString dictionaryFileName;

public slots:
    void onDialogAccepted();

private:
    Ui::DictionaryDialog *ui;
};

#endif // DICTIONARYDIALOG_H
