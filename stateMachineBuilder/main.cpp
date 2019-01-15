#include "mainwindow.h"
#include <QApplication>
#include "parser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Parser parser;
    parser.readText();
    parser.deleteComments();
    parser.deleteBadStrings();

    return a.exec();
}
