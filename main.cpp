#include "dialog.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //glutInit()
    QApplication a(argc, argv);
    MainWindow mw;
    Dialog w;
    mw.show();
    w.show();
    return a.exec();
}
