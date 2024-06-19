#ifndef USERLIST_H
#define USERLIST_H

#include "notification.h"
#include <QDialog>
#include <QListWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class UserList;
}

class UserList : public QDialog {
    Q_OBJECT

public:
    explicit UserList(QWidget *parent = nullptr);
    static void dbg(QString str) {
        qDebug() << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "] [INFO] " << str;
    }
    ~UserList();

private slots:
    void onAccountClicked(QListWidgetItem *item);
    void onAccountsFetched(QNetworkReply *reply);

private:
    void fetchAccounts();
    Ui::UserList *ui;
    QNetworkAccessManager *networkManager;
    Notification *notification = new Notification;
};

#endif // USERLIST_H
