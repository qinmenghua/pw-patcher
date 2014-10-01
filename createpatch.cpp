#include <QDir>
#include <QTimer>
#include <QMessageBox>
#include "createpatch.h"
#include "ui_createpatch.h"

CreatePatch::CreatePatch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CreatePatch),
    task(NULL)
{
    ui->setupUi(this);
}

CreatePatch::~CreatePatch()
{
    delete ui;
}

void CreatePatch::createPatch(const QString &sourceFolder)
{
    if (!QDir(sourceFolder).exists())
    {
        close();
        QMessageBox::critical(this, "INVALID FOLDER", "Folder gak ada:\n" + sourceFolder);
        exit(EXIT_FAILURE);
        return;
    }

    QString fileName(sourceFolder + ".sempack");
    if (QFile::exists(fileName))
    {
        close();
        QMessageBox::critical(this, "FILE EXISTS", "File patch sudah ada:\n" + fileName);
        exit(EXIT_FAILURE);
        return;
    }

    ui->sourceFolderText->setPlainText(sourceFolder);

    // sPCK uses sza.exe to compress file
    // since there is no (easy) way to also monitor sza.exe that's spawned by sPCK,
    // we use timer to find out whether or not sPCK is still processing or already
    // spawn sza to do next task
    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(3000);
    connect(timer, &QTimer::timeout, [this]() {
        ui->logViewText->appendPlainText("\n\nCompressing Files...\n");
    });

    QStringList args;
    args << "-pw" << "-cp" << sourceFolder;

    task = new QProcess(this);
    task->setProcessChannelMode(QProcess::MergedChannels);

    connect(task, &QProcess::readyRead, [this, timer]() {
        char buf[512];
        qint64 bytesRed = task->readLine(buf, sizeof(buf));
        if (bytesRed != -1)
        {
            QString line(buf);
            if (line.endsWith('\n'))
                ui->logViewText->appendPlainText(line);
        }

        // sPCK still processing, restart timer
        timer->start();
    });

    void (QProcess::*finishedSignal)(int, QProcess::ExitStatus) = &QProcess::finished;
    connect(task, finishedSignal, [this, sourceFolder](int, QProcess::ExitStatus){
        task->deleteLater();

        QString dstDir = sourceFolder.left(sourceFolder.length()-1).left(sourceFolder.lastIndexOf('\\'));
        QString target = sourceFolder.right(sourceFolder.length() - sourceFolder.lastIndexOf('\\'));

        QString resultFile(dstDir + target + ".sempack");
        QFile::rename(dstDir + "\\ec_patch_0-0.xup", resultFile);

        QMessageBox::information(this, "FINISHED", "Patch File Created At:\n" + resultFile);
        QApplication::quit();
    });

    task->start("bin\\sPCK.exe", args);
}
