#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QJsonArray>

ChatWindow::ChatWindow(const QString &username, QWidget *parent) : QMainWindow(parent), ui(new Ui::ChatWindow), networkManager(new QNetworkAccessManager(this)), username(username) {
    dbg("init chat window");
    setFixedSize(651, 429);
    ui->setupUi(this);

    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);
    messageInput = new QLineEdit(this);
    sendButton = new QPushButton("Send", this);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(chatDisplay);
    layout->addWidget(messageInput);
    layout->addWidget(sendButton);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    dbg("Connect signals and slots");
    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::sendMessage);
    connect(messageInput, &QLineEdit::returnPressed, this, &ChatWindow::sendMessage);
    connect(networkManager, &QNetworkAccessManager::finished, this, &ChatWindow::handleNetworkReply);
    dbg("Set window title");
    setWindowTitle("Chat with " + username);
}

ChatWindow::~ChatWindow() {
    delete ui;
}

void ChatWindow::sendMessage() {
    dbg("Sending message...");
    QString message = messageInput->text();
    if (!message.isEmpty()) {
        dbg("Creating JSON object...");
        QJsonObject json;
        json["message"] = message;
        json["username"] = username;
        json["method"] = "chat";

        dbg("Convert JSON object to QByteArray");
        QJsonDocument jsonDoc(json);
        QByteArray jsonData = jsonDoc.toJson();

        dbg("Create network request");
        QNetworkRequest request((QUrl(serverUrl)));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        dbg("Send POST request");
        networkManager->post(request, jsonData);

        messageInput->clear();
    }
}

void ChatWindow::handleNetworkReply(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObject = jsonResponse.object();

        QString message = jsonObject["message"].toString();
        QString timeStr = jsonObject["timestamp"].toString();
        QDateTime timestamp = QDateTime::fromString(timeStr, Qt::ISODate);

        messages.append(qMakePair(timestamp, message));

        dbg("Sort messages by timestamp");
        std::sort(messages.begin(), messages.end(), [](const QPair<QDateTime, QString> &a, const QPair<QDateTime, QString> &b) {
            return a.first < b.first;
        });

        dbg("Display Message");
        displayMessages();
    }
    else chatDisplay->append("<font color=\"red\">Error: " + reply->errorString() + "</font>");

    reply->deleteLater();
}

void ChatWindow::displayMessages() {
    chatDisplay->clear();
    for (const auto &message : messages) {
        QString timeStr = message.first.toString("yyyy-MM-dd HH:mm:ss");
        chatDisplay->append("[" + timeStr + "] " + message.second);
    }
}
