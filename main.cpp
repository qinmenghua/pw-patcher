#include "mainwindow.h"
#include <iostream>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QStringList>

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        QString sourceDir(argv[1]);
        if (!QDir(sourceDir).exists())
        {
            std::cout << "Not a valid Directory: " << argv[1] << std::endl;
            return 1;
        }

        std::cout << "Creating Patch..." << std::endl;

        QStringList args;
        args << "-pw" << "-cp" << sourceDir;
        QProcess::execute("bin\\sPCK.exe", args);

        QString dstDir = sourceDir.left(sourceDir.length()-1).left(sourceDir.lastIndexOf('\\'));
        QString target = sourceDir.right(sourceDir.length() - sourceDir.lastIndexOf('\\'));

        QFile::rename(dstDir + "\\ec_patch_0-0.xup", dstDir + target + ".sempack");

        return 0;
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
