#ifndef CREATE_LNK_FILE_H
#define CREATE_LNK_FILE_H

#include <QDir>
#include <QString>
#include <QTemporaryFile>
#include <QProcess>
#include <QStandardPaths>

//win下 创建带参数的快捷方式
// createLnkFile("E:\\Brisa\\aivctrl.exe", "-r -s xx -d", "E:\\Brisa\\xx.lnk");
inline void createLnkFile(const QString &fileName, const QString &args, const QString &lnkName)
{
    QString vbsFile;
    {
        const QString path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        QTemporaryFile fi(path + "/lnk_XXXXXXXXXX_tmp.vbs");
        fi.setAutoRemove(false);
        if(fi.open())
        {
            vbsFile = fi.fileName();

            fi.write("Set Shell = CreateObject(\"WScript.Shell\")\r\n");

            fi.write("Set link = Shell.CreateShortcut(\"");
            fi.write(lnkName.toUtf8());
            fi.write("\")\r\n");

            fi.write("link.Arguments = \"");
            fi.write(args.toUtf8());
            fi.write("\"\r\n");

            fi.write("link.IconLocation = \"");
            fi.write(fileName.toUtf8());
            fi.write(",0\"\r\n");

            fi.write("link.TargetPath = \"");
            fi.write(fileName.toUtf8());
            fi.write("\"\r\n");

            QFileInfo fi_info(fileName);
            fi.write("link.WorkingDirectory = \"");
            fi.write(QDir::toNativeSeparators(fi_info.filePath()).toUtf8());
            fi.write("\"\r\n");

            fi.write("link.Description = \"lnk file.\"\r\n");
            fi.write("link.HotKey = \"\"\r\n");
            fi.write("link.WindowStyle = 1\r\n");
            fi.write("link.Save");

            fi.flush();
            fi.close();
        }
    }

    {
        QProcess pro;
        pro.start("cmd.exe /c " + vbsFile);
        //pro.start("PowerShell.exe /c " + vbsFile);
        pro.waitForFinished();

        QFile::remove(vbsFile);
    }
}

#endif // CREATE_LNK_FILE_H
