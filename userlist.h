#ifndef USERLIST_H
#define USERLIST_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QListWidgetItem>

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

public slots:
    void fetchAccounts();
    void onAccountsFetched(QNetworkReply *reply);
    void onAccountClicked(QListWidgetItem *item);

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::UserList *ui;
    QNetworkAccessManager *networkManager;
    const QString serverUrl = "http://your-server-url.com"; // Adjust as needed
};

#endif // USERLIST_H
