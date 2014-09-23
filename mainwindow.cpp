#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>

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
    if (event->mimeData()->urls().count() != 1)
    {
        event->acceptProposedAction();

        QMessageBox m;
        m.setText(event->mimeData()->text());
        m.exec();
    }
}

void MainWindow::on_btnExit_clicked()
{
    QApplication::quit();
}

void MainWindow::on_btnPatch_clicked()
{
    QFileDialog dlg(this);
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setFileMode(QFileDialog::ExistingFile);
    dlg.setNameFilter("PW Scarlet Patch (*.sawer *.sempack)");

    if (dlg.exec())
    {
        QString fileName = dlg.selectedFiles().at(0);

        QMessageBox m;
        m.setText(fileName);
        m.exec();
    }
}
