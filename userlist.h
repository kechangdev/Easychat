#ifndef USERLIST_H
#define USERLIST_H

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
    ~UserList();

private slots:
    void onAccountClicked(QListWidgetItem *item);
    void onAccountsFetched(QNetworkReply *reply);

private:
    void fetchAccounts();

    Ui::UserList *ui;
    QNetworkAccessManager *networkManager;
};

#endif // USERLIST_H
