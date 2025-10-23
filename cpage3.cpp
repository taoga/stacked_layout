#include "cpage3.h"
#include "ui_cpage3.h"

CPage3::CPage3(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CPage3)
{
    ui->setupUi(this);
}

CPage3::~CPage3()
{
    delete ui;
}
