#include "ui/mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QLocale::setDefault(QLocale(QLocale::Russian,QLocale::RussianFederation));
    QString locale = QLocale::system().name();
    QTranslator qtTranslator;
    if (qtTranslator.load("qt_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
         a.installTranslator(&qtTranslator);

    MainWindow w;
    w.show();
    return a.exec();
}
