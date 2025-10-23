#include "cpage1.h"
#include "ui_cpage1.h"

CPage1::CPage1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CPage1)
{
    ui->setupUi(this);
}

CPage1::~CPage1()
{
    delete ui;
}
