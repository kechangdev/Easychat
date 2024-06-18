#include "login_window.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    login_window loginwindow;
    loginwindow.show();
    return a.exec();
}
