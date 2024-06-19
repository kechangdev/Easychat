#include "userlist.h"
#include "ui_userlist.h"
#include "chatwindow.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QListWidget>
#include <QDebug>

UserList::UserList(QWidget *parent) : QDialog(parent), ui(new Ui::UserList), networkManager(new QNetworkAccessManager(this)) {
    dbg("Initializing user list window");
    setFixedSize(651, 429);
    ui->setupUi(this);

    dbg("Setting up signal-slot connections");
    connect(ui->accountListWidget, &QListWidget::itemClicked, this, &UserList::onAccountClicked);
    connect(networkManager, &QNetworkAccessManager::finished, this, &UserList::onAccountsFetched);
}

UserList::~UserList() {
    delete ui;
    delete networkManager;
}

void UserList::showEvent(QShowEvent *event) {
    QDialog::showEvent(event);
    dbg("Showing user list window");
    fetchAccounts();
}

void UserList::fetchAccounts() {
    dbg("Preparing request...");
    QNetworkRequest request{QUrl(serverUrl)};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["method"] = "userlist";
    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson();

    dbg("Sending request to server...");
    networkManager->post(request, jsonData);
}

void UserList::onAccountsFetched(QNetworkReply *reply) {
    dbg("Received response from server");
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if (!jsonDoc.isArray()) {
            dbg("Response is not a JSON array");
            reply->deleteLater();
            return;
        }

        QJsonArray jsonArray = jsonDoc.array();
        dbg("Populating user list...");
        ui->accountListWidget->clear();
        for (const QJsonValue &value : jsonArray) {
            QJsonObject obj = value.toObject();
            ui->accountListWidget->addItem(obj["username"].toString());
        }
        dbg("User list populated");
    }
    else dbg("Network error: " + reply->errorString());
    reply->deleteLater();
}

void UserList::onAccountClicked(QListWidgetItem *item) {
    QString username = item->text();
    ChatWindow *chatWindow = new ChatWindow(username, this);
    chatWindow->getUsername(usernameA);
    chatWindow->show();
}
