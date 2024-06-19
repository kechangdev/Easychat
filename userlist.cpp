#include "userlist.h"
#include "ui_userlist.h"
#include "chatwindow.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

UserList::UserList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserList),
    networkManager(new QNetworkAccessManager(this)) {

    ui->setupUi(this);

    connect(ui->accountListWidget, &QListWidget::itemClicked, this, &UserList::onAccountClicked);
    connect(networkManager, &QNetworkAccessManager::finished, this, &UserList::onAccountsFetched);

    fetchAccounts();
}

UserList::~UserList() {
    delete ui;
}

void UserList::fetchAccounts() {
    QNetworkRequest request(QUrl("http://kechang.fun:5490/getAccounts"));
    networkManager->get(request);
}

void UserList::onAccountsFetched(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonArray jsonArray = jsonDoc.array();

        ui->accountListWidget->clear();
        for (const QJsonValue &value : jsonArray) {
            QJsonObject obj = value.toObject();
            ui->accountListWidget->addItem(obj["username"].toString());
        }
    } else {
        // Handle error
    }
    reply->deleteLater();
}

void UserList::onAccountClicked(QListWidgetItem *item) {
    QString username = item->text();
    ChatWindow *chatWindow = new ChatWindow(username, this);
    chatWindow->show();
}
