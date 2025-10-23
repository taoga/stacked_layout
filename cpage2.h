#ifndef CPAGE2_H
#define CPAGE2_H

#include <QWidget>

namespace Ui {
class CPage2;
}

class CPage2 : public QWidget
{
    Q_OBJECT

public:
    explicit CPage2(QWidget *parent = nullptr);
    ~CPage2();

private:
    Ui::CPage2 *ui;
};

#endif // CPAGE2_H
