#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H

#include "notification.h"
#include <QDialog>
#include <QDebug>
#include <QDateTime>
#include <QNetworkAccessManager>

namespace Ui {
class SignupWindow;
}

class SignupWindow : public QDialog {
    Q_OBJECT

public:
    explicit SignupWindow(QWidget *parent = nullptr);
    ~SignupWindow();
    void getUsername(QString& _usernameA){usernameA = _usernameA;}
    static void dbg(const QString &str) {
        qDebug() << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "] [INFO] " << str;
    }

private slots:
    void onAccepted();
    bool signup_Response_Check(const QByteArray &responseData);

private:
    Ui::SignupWindow *ui;
    QNetworkAccessManager *networkManager;
    const QString serverUrl = "http://49.234.177.172:10086/api";
    Notification *notification;
    QString usernameA;
};

#endif // SIGNUPWINDOW_H
