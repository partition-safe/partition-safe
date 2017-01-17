#include "mainwindow/mainwindow.h"
#include <QApplication>
#include <time.h>

/**
 * Gets called when the program starts and opens the MainWindow
 * @brief qMain
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    try {
        srand(time(0));
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
