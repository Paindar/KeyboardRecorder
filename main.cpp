#include "mainwindow.h"
#include "globals.h"
#include "keyboardthread.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::addLibraryPath("./plugins");
    QApplication a(argc, argv);
    MainWindow w;
    KeyboardThread thread(&a);
    loadStorage();
    w.flushTable();
    kThread=&thread;
    QObject::connect(&thread,&KeyboardThread::fresh,&w,&MainWindow::fresh);
    thread.start();
    w.show();

    return a.exec();
}
