#ifndef CPAGE1_H
#define CPAGE1_H

#include <QWidget>

namespace Ui {
class CPage1;
}

class CPage1 : public QWidget
{
    Q_OBJECT

public:
    explicit CPage1(QWidget *parent = nullptr);
    ~CPage1();

private:
    Ui::CPage1 *ui;
};

#endif // CPAGE1_H
