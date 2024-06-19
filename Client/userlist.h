#ifndef USERLIST_H
#define USERLIST_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QListWidgetItem>
#include <QDateTime>
#include <QDebug>

namespace Ui {
class UserList;
}

class UserList : public QDialog {
    Q_OBJECT

public:
    explicit UserList(QWidget *parent = nullptr);
    void getUsername(QString& _usernameA){usernameA = _usernameA;}
    ~UserList();

public slots:
    void fetchAccounts();
    void onAccountsFetched(QNetworkReply *reply);
    void onAccountClicked(QListWidgetItem *item);

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::UserList *ui;
    QNetworkAccessManager *networkManager;
    const QString serverUrl = "http://49.234.177.172:10086/api";
    QString usernameA;
    static void dbg(const QString &str) {
        qDebug() << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "] [INFO] " << str;
    }
};

#endif // USERLIST_H
