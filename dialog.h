#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "cpage1.h"
#include "cpage2.h"
#include "cpage3.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

//private slots:
//    void on_pushButton_clicked();
//    void on_pushButton_2_clicked();
//    void on_pushButton_3_clicked();

private:
    Ui::Dialog  *ui;
    //
    CPage1      *m_pwgtPage1;
    CPage2      *m_pwgtPage2;
    CPage3      *m_pwgtPage3;
};
#endif // DIALOG_H
