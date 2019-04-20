#include "scandir.h"
#include <QFileInfo>
#include <QDir>

ScanDir::ScanDir(QObject *parent) : QThread(parent)
{

}

void ScanDir::scan(const QString &path, const QStringList &filters)
{
    if(this->isRunning())
        return;
    m_scanedFiles.clear();
    if(path.endsWith("/"))
        m_path = path.left(-1);
    else
        m_path = path;
    m_filters = filters;
    this->start();
}

void ScanDir::stop()
{
    this->requestInterruption();
}

void ScanDir::run()
{
    m_scanedFiles = getFiles(m_path, m_filters);
    emit scanedFinish();
}

QStringList ScanDir::getFiles(const QString &path, const QStringList &filters)
{
    QStringList list;

    QDir dir;
    dir.setPath(path);
    QFileInfoList infolist = dir.entryInfoList(filters,
                                               QDir::AllDirs|QDir::Files|QDir::NoDotAndDotDot);

    foreach (const QFileInfo &info, infolist)
    {
        if(this->isInterruptionRequested())
            break;
        if(info.isDir())
        {
            list << getFiles(dir.absolutePath() + "/" + info.fileName(), filters);
        }
        else
            list << info.absoluteFilePath();
    }

    return list;
}
