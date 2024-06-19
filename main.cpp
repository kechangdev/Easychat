#include "login_window.h"
#include "signupwindow.h"
#include "chatwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    login_window loginwindow;
    // SignupWindow signupwindow;
    //ChatWindow cw;
    loginwindow.show();
    //cw.show();
    return a.exec();
}
