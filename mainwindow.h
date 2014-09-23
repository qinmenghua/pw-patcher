#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "animatedlabel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:

    void on_btnExit_clicked();

private:
    Ui::MainWindow *ui;
    QPoint dragPosition;

};

#endif // MAINWINDOW_H
