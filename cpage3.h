#ifndef CPAGE3_H
#define CPAGE3_H

#include <QWidget>

namespace Ui {
class CPage3;
}

class CPage3 : public QWidget
{
    Q_OBJECT

public:
    explicit CPage3(QWidget *parent = nullptr);
    ~CPage3();

private:
    Ui::CPage3 *ui;
};

#endif // CPAGE3_H
