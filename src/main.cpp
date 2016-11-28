#include "mainwindow/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    try {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    } catch(const char *exception) {
        std::cout << exception << std::endl;
        return 1;
    } catch(...) {
        return 2;
    }
}
