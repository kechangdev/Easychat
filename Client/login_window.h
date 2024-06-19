#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H

#include <QMainWindow>
#include <QString>
#include <QNetworkAccessManager>
#include "signupwindow.h"
#include "notification.h"
#include "userlist.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class login_window;
}
QT_END_NAMESPACE

class login_window : public QMainWindow {
    Q_OBJECT

public:
    explicit login_window(QWidget *parent = nullptr);
    static void dbg(const QString &str) {
        qDebug() << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "] [INFO]" << str;
    }
    ~login_window();

private slots:
    void onLoginButtonClicked();
    void onReplyFinished(QNetworkReply *reply);
    bool loginResponseCheck(const QByteArray &responseData);
    void onSignupLinkClicked();

private:
    Ui::login_window *ui;
    SignupWindow *signupwindow;
    QNetworkAccessManager *networkManager;
    const QString serverUrl = "https://chat.6b.fit";
    Notification *notification;
    UserList *userlist;

    QString usernameA;
};

#endif // LOGIN_WINDOW_H
