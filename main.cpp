#include "dialog.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    QApplication a(argc, argv);

    QTranslator traпslator;
    traпslator.load("QtLanguage_en.qm", ".");
    qApp->installTranslator(&traпslator);

    Dialog w;
    w.show();
    return a.exec();
}
