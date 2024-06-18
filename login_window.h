#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H

#include <QMainWindow>
#include <QString>
#include <QNetworkAccessManager>
#include "signupwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class login_window;
}
QT_END_NAMESPACE

class login_window : public QMainWindow {
    Q_OBJECT

public:
    login_window(QWidget *parent = nullptr);
    void dbg(QString str);
    ~login_window();

private slots:
    void onLoginButtonClicked();
    void onReplyFinished(QNetworkReply *reply);
    bool login_Response_Check(QByteArray& responseData);
    void onSignupLinkClicked(const QString &link);

private:
    Ui::login_window *ui;
    SignupWindow * signupwindow;
    QNetworkAccessManager *networkManager;
    QString Server_URL = "http://kechang.fun";
};
#endif // LOGIN_WINDOW_H
