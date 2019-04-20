#include <QCoreApplication>
#include <QtDebug>


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    //默认升序
    QStringList list;
    list << "Zero" << "AlPha" << "beTA" << "gamma" << "DELTA";

    qSort(list.begin(), list.end(), qGreater<QString>() ); //降序
    qDebug() << list;

    qSort(list.begin(), list.end(), qLess<QString>() );    //升序
    qDebug() << list;

    return app.exec();
}
