#include "userlist.h"
#include "ui_userlist.h"
#include "chatwindow.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QListWidget>
#include <QDebug>

UserList::UserList(QWidget *parent) : QDialog(parent), ui(new Ui::UserList), networkManager(new QNetworkAccessManager(this)) {
    dbg("init userlist window");
    setFixedSize(651, 429);
    ui->setupUi(this);

    dbg("init signal & slots");
    connect(ui->accountListWidget, &QListWidget::itemClicked, this, &UserList::onAccountClicked);
    connect(networkManager, &QNetworkAccessManager::finished, this, &UserList::onAccountsFetched);
}

UserList::~UserList() {
    delete ui;
    delete networkManager;
}

void UserList::showEvent(QShowEvent *event) {
    QDialog::showEvent(event);
    dbg("Push userlist:");
    fetchAccounts();
}

void UserList::fetchAccounts() {
    dbg("Prepare request...");
    QNetworkRequest request{QUrl(serverUrl)};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["method"] = "userlist";
    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson();

    dbg("Send request to server...");
    networkManager->post(request, jsonData);
}

void UserList::onAccountsFetched(QNetworkReply *reply) {
    dbg("Geted response from server");
    dbg("Analysis response");
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonArray jsonArray = jsonDoc.array();
        dbg("Listing userlist..");
        ui->accountListWidget->clear();
        for (const QJsonValue &value : jsonArray) {
            QJsonObject obj = value.toObject();
            ui->accountListWidget->addItem(obj["username"].toString());
        }
        dbg("Userlist has Listed!");
    }
    else dbg("Network error: " + reply->errorString());
    reply->deleteLater();
}

void UserList::onAccountClicked(QListWidgetItem *item) {
    QString username = item->text();
    ChatWindow *chatWindow = new ChatWindow(username, this);
    chatWindow->show();
}
