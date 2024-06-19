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
    static void dbg(QString str) {
        qDebug() << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "] [INFO] " << str;
    }

private slots:
    void onAccepted();
    bool signup_Response_Check(QByteArray& responseData);

private:
    Ui::SignupWindow *ui;
    QNetworkAccessManager *networkManager;
    const QString serverUrl = "https://chat.6b.fit/";
    Notification *notification = new Notification;
};

#endif // SIGNUPWINDOW_H
