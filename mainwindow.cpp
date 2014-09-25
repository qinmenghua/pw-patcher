#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QFutureWatcher>
#include <QtConcurrent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    refTask(NULL)
{

    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);

    setAcceptDrops(true);

    ui->animation->initialize(":/img/animation/progress", 15, 3, 14);
    ui->animation->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    event->acceptProposedAction();

    if (event->mimeData()->urls().count() != 1)
    {
        QMessageBox::warning(this, "ERROR", "Sabar kk, file patch nya satu - persatu yah...");
        return;
    }

    QUrl url = event->mimeData()->urls().at(0);
    if (!url.isLocalFile())
    {
        QMessageBox::critical(this, "NON LOCAL FILE", "URL IS NON LOCAL FILE");
        return;
    }

    QString path = QDir::toNativeSeparators(url.toLocalFile());
    if (!path.endsWith(".sempack"))
    {
        QMessageBox::critical(this, "INVALID FORMAT", "Sepertinya ini bukan file patch PWScarlet, deh");
        return;
    }

    applyPatch(path);
}

void MainWindow::on_btnExit_clicked()
{
    if (refTask && refTask->processId() != 0)
        refTask->kill();

    QApplication::quit();
}

void MainWindow::on_btnStart_clicked()
{
    QStringList args;
    args << "startbypatcher";

    QProcess::startDetached("..\\element\\elementclient.exe", args, "..\\element\\");
    QThread::sleep(3);

    QApplication::quit();
}

void MainWindow::on_btnPatch_clicked()
{
    QFileDialog dlg(this);
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setFileMode(QFileDialog::ExistingFile);
    dlg.setNameFilter("PWScarlet Patch (*.sempack)");

    if (dlg.exec())
    {
        QString fileName = dlg.selectedFiles().at(0);
        if (!fileName.endsWith(".sempack"))
        {
            QMessageBox::critical(this, "INVALID FORMAT", "Sepertinya ini bukan file patch PWScarlet, deh");
            return;
        }
        applyPatch(fileName);
    }
}

void MainWindow::applyPatch(const QString &fileName)
{
    ui->btnPatch->setEnabled(false);
    ui->btnStart->setEnabled(false);

    ui->animation->start();

    QFutureWatcher<void> *future = new QFutureWatcher<void>();
    connect(future, &QFutureWatcher<void>::finished, [this,future]() {
        ui->animation->stop();

        ui->btnStart->setEnabled(true);
        ui->btnPatch->setEnabled(true);

        refTask = NULL;

        future->deleteLater();
    });

    future->setFuture(QtConcurrent::run([=](const QString &fileName) {
        QStringList args;
        QProcess task; refTask = &task;

        args << "x" << fileName << "-y" << "-o..\\";
        task.start("bin\\sza.exe", args);
        task.waitForFinished(-1);

        args.clear(); args << "*.pck.b64.files";
        QDir elementDir("..\\element");
        QStringList packs = elementDir.entryList(args, QDir::Dirs);

        for (int i = 0; i < packs.size(); i++)
        {
            QString source = packs.at(i).toLocal8Bit().constData();
            QString dest = source.left(source.indexOf('.'));
            QString pck = dest + ".pck";
            QString pkx = dest + ".pkx";

            QString originalWorkingDir = QDir::current().dirName();
            QDir::setCurrent("..\\element");

            if (QFile::exists(pkx))
            {
                // TODO: merge *.pck and *.pkx
                args.clear(); args << "/c" << "copy" << "/b" << pck+"+"+pkx << pck;
                task.start("cmd.exe", args);
                task.waitForFinished(-1);
                QFile::remove(pkx);
            }

            // TODO: merge source folder into *.pck
            args.clear(); args << "-pw" << "-ap" << source;
            task.start("bin\\sPCK.exe", args);
            task.waitForFinished(-1);

            // TODO: split *.pck into *.pkx
            if (QFile(pck).size() >= 2147483648)
            {
                args.clear(); args << pck << "2147483648";
                task.start("bin\\split.exe", args);
                task.waitForFinished(-1);
                QFile::remove(pck + ".1");
                QFile::rename(pck + ".2", pkx);
            }

            // TODO: delete source folder recursive
            QDir(source).removeRecursively();


            QDir::setCurrent(originalWorkingDir);
        }

    }, fileName));
}
