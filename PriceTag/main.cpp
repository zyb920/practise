#include "Widget.h"
#include <QApplication>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    //QApplication::addLibraryPath("./plugins");

    Widget w;
    w.show();

    return a.exec();
}
