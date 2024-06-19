#include "login_window.h"
#include "signupwindow.h"
#include "chatwindow.h"
#include "userlist.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    login_window loginwindow;
    // SignupWindow signupwindow;
    // ChatWindow *cw = new ChatWindow("ChatGPT", new UserList);
    // cw->show();
    // UserList ul;
    // ul.show();
    // cw.show();
    loginwindow.show();
    return a.exec();
}
