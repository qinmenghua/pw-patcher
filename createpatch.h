#ifndef CREATEPATCH_H
#define CREATEPATCH_H

#include <QMainWindow>
#include <QProcess>

namespace Ui {
class CreatePatch;
}

class CreatePatch : public QMainWindow
{
    Q_OBJECT

public:
    explicit CreatePatch(QWidget *parent = 0);
    ~CreatePatch();
    void createPatch(const QString &sourceFolder);

private:
    Ui::CreatePatch *ui;
    QProcess *task;
};

#endif // CREATEPATCH_H
