#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "notification.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QJsonArray>

ChatWindow::ChatWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ChatWindow), networkManager(new QNetworkAccessManager(this)), username(username) {
    setFixedSize(651, 429);
    ui->setupUi(this);

    // Initialize widgets
    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);

    messageInput = new QLineEdit(this);

    sendButton = new QPushButton("Send", this);

    // Layout setup
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(chatDisplay);
    layout->addWidget(messageInput);
    layout->addWidget(sendButton);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Connect signals and slots
    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::sendMessage);
    connect(messageInput, &QLineEdit::returnPressed, this, &ChatWindow::sendMessage);
    connect(networkManager, &QNetworkAccessManager::finished, this, &ChatWindow::handleNetworkReply);

    setWindowTitle("Chat with " + username);
}

ChatWindow::~ChatWindow() {
    delete ui;
}

void ChatWindow::sendMessage() {
    QString message = messageInput->text();
    if (!message.isEmpty()) {
        // Create JSON object
        QJsonObject json;
        json["message"] = message;
        json["username"] = username;

        // Convert JSON object to QByteArray
        QJsonDocument jsonDoc(json);
        QByteArray jsonData = jsonDoc.toJson();

        // Create network request
        QNetworkRequest request((QUrl(serverUrl)));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        // Send POST request
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
        QString timeStr = jsonObject["timestamp"].toString(); // 假设服务器返回的JSON中有timestamp字段
        QDateTime timestamp = QDateTime::fromString(timeStr, Qt::ISODate);

        messages.append(qMakePair(timestamp, message));

        // Sort messages by timestamp
        std::sort(messages.begin(), messages.end(), [](const QPair<QDateTime, QString> &a, const QPair<QDateTime, QString> &b) {
            return a.first < b.first;
        });

        displayMessages();
    } else {
        chatDisplay->append("<font color=\"red\">Error: " + reply->errorString() + "</font>");
    }

    reply->deleteLater();
}

void ChatWindow::displayMessages() {
    chatDisplay->clear();
    for (const auto &message : messages) {
        QString timeStr = message.first.toString("yyyy-MM-dd HH:mm:ss");
        chatDisplay->append("[" + timeStr + "] " + message.second);
    }
}
