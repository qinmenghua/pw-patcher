#include "mainwindow.h"
#include "createpatch.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        QApplication a(argc, argv);
        CreatePatch p;
        p.show();
        p.createPatch(argv[1]);

        return a.exec();
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
