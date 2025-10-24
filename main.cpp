#include "dialog.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // need fix
#else
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
#endif
    QApplication a(argc, argv);

    QTranslator traпslator;
    traпslator.load("QtLanguage_en.qm", ".");
    qApp->installTranslator(&traпslator);

    Dialog w;
    w.show();
    return a.exec();
}
