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
    const QString serverUrl = "https://chat.6b.fit";
};

#endif // USERLIST_H
