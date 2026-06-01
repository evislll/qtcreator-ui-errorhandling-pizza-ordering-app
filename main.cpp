#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set application style
    a.setStyle("Fusion");

    MainWindow w;
    w.resize(600, 700);
    w.show();

    return a.exec();
}
