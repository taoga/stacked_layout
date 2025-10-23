#include "dialog.h"
#include "ui_dialog.h"
#include <QVBoxLayout>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    m_pwgtPage1 = new CPage1( ui->ribbonOpts->GetContainer() );
    m_pwgtPage2 = new CPage2( ui->ribbonOpts->GetContainer() );
    m_pwgtPage3 = new CPage3( ui->ribbonOpts->GetContainer() );

    ui->ribbonOpts->AddOpt( m_pwgtPage1, "RX", ":/images/recv.png", ":/images/recv_sel.png" );
    ui->ribbonOpts->AddOpt( m_pwgtPage2, "TX", ":/images/trans.png", ":/images/trans_sel.png" );
    ui->ribbonOpts->AddOpt( m_pwgtPage3, QObject::tr("Общие"), ":/images/settings.png", ":/images/settings_sel.png" );

    ui->ribbonOpts->SetContainer();
}

Dialog::~Dialog()
{
    delete ui;
}
