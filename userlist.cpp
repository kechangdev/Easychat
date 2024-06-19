#include "userlist.h"
#include "ui_userlist.h"
#include "chatwindow.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QListWidget>
#include <QDebug>

UserList::UserList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserList),
    networkManager(new QNetworkAccessManager(this)) {
    setFixedSize(651, 429);
    ui->setupUi(this);

    connect(ui->accountListWidget, &QListWidget::itemClicked, this, &UserList::onAccountClicked);
    connect(networkManager, &QNetworkAccessManager::finished, this, &UserList::onAccountsFetched);
}

UserList::~UserList() {
    delete ui;
    delete networkManager;
}

void UserList::showEvent(QShowEvent *event) {
    QDialog::showEvent(event);
    fetchAccounts();
}

void UserList::fetchAccounts() {
    QNetworkRequest request{QUrl(serverUrl)};
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
        qDebug() << "Network error:" << reply->errorString();
    }
    reply->deleteLater();
}

void UserList::onAccountClicked(QListWidgetItem *item) {
    QString username = item->text();
    ChatWindow *chatWindow = new ChatWindow(username, this);
    chatWindow->show();
}
