#include "login_window.h"
#include "signupwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    login_window loginwindow;
    SignupWindow signupwindow;
    loginwindow.show();
    return a.exec();
}
