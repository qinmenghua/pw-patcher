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
    ui(new Ui::MainWindow)
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
    if (!path.endsWith(".sempack") && !path.endsWith(".sawer"))
    {
        QMessageBox::critical(this, "INVALID FORMAT", "Sepertinya ini bukan file patch PWScarlet, deh");
        return;
    }

    applyPatch(path);
}

void MainWindow::on_btnExit_clicked()
{
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
    dlg.setNameFilter("PWScarlet Patch (*.sawer *.sempack)");

    if (dlg.exec())
    {
        QString fileName = dlg.selectedFiles().at(0);
        if (!fileName.endsWith(".sempack") && !fileName.endsWith(".sawer"))
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

        future->deleteLater();
    });

    future->setFuture(QtConcurrent::run([=](const QString &fileName) {
        QStringList args;
        args << fileName;

        QProcess notepad;
        notepad.start("notepad.exe", args);
        notepad.waitForFinished();

        // TODO: call sza.exe to extract into directory

        // TODO: check applicable *.pck
        // TODO:   merge *.pck and *.pkx
        // TODO:   call spck.exe to merge *.pck
        // TODO:   split *.pck into *.pkx


    }, fileName));


}
