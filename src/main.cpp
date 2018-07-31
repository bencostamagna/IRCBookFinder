
#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<QList<SearchResult> >("QList<SearchResult>");

    MainWindow window;
   
    window.show();

    return a.exec();
}

