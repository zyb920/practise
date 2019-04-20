#ifndef SCANDIR_H
#define SCANDIR_H

#include <QThread>

class ScanDir : public QThread
{
    Q_OBJECT
public:
    explicit ScanDir(QObject *parent = nullptr);

    void scan(const QString &path, const QStringList &filters);
    QStringList scanedFiles() const
    {
        return m_scanedFiles;
    }
    void stop();

signals:
    void scanedFinish();

protected:
    void run() override;

private:
    QStringList getFiles(const QString &path, const QStringList &filters);

private:
    QString m_path;
    QStringList m_filters;
    QStringList m_scanedFiles;
};

#endif // SCANDIR_H
