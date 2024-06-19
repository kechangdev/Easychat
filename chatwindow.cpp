#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>

ChatWindow::ChatWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ChatWindow), networkManager(new QNetworkAccessManager(this)), username(username) {

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
        chatDisplay->append("Me: " + message);

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
        QString chatGptMessage = jsonObject["response"].toString(); // 假设服务器返回的JSON中有response字段
        chatDisplay->append("<font color=\"red\">ChatGPT: " + chatGptMessage + "</font>");
    } else {
        chatDisplay->append("<font color=\"red\">Error: " + reply->errorString() + "</font>");
    }

    reply->deleteLater();
}
