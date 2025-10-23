#include "cpage2.h"
#include "ui_cpage2.h"

CPage2::CPage2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CPage2)
{
    ui->setupUi(this);
}

CPage2::~CPage2()
{
    delete ui;
}
