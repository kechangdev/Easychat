#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QJsonArray>

ChatWindow::ChatWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ChatWindow), networkManager(new QNetworkAccessManager(this)), username(username), notification(new Notification), pollingTimer(new QTimer(this))
{
    dbg("Initializing chat window");
    setFixedSize(651, 429);
    ui->setupUi(this);

    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);
    messageInput = new QLineEdit(this);
    sendButton = new QPushButton("Send", this);
    clearButton = new QToolButton(this);
    clearButton->setText("Clear History");
    clearButton->setIcon(QIcon(":/icons/clear.png"));
    clearButton->setAutoRaise(true);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(chatDisplay);
    layout->addWidget(messageInput);
    layout->addWidget(sendButton);
    layout->addWidget(clearButton);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    dbg("Connecting signals and slots");
    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::sendMessage);
    connect(messageInput, &QLineEdit::returnPressed, this, &ChatWindow::sendMessage);
    connect(networkManager, &QNetworkAccessManager::finished, this, &ChatWindow::handleNetworkReply);
    connect(clearButton, &QToolButton::clicked, this, &ChatWindow::clearChatHistory);
    connect(pollingTimer, &QTimer::timeout, this, &ChatWindow::getChatHistory);

    dbg("Setting window title");
    setWindowTitle("Chat with " + username);

    dbg("Fetch chat history when the window is initialized");
    getChatHistory();

    dbg("Start polling for chat history every 5 seconds");
    startPolling();
}

ChatWindow::~ChatWindow() {
    delete ui;
    delete notification;
    stopPolling();
}

void ChatWindow::sendMessage() {
    dbg("Sending message...");
    QString message = messageInput->text();
    if (!message.isEmpty()) {
        dbg("Creating JSON object...");
        QJsonObject json;
        json["message"] = message;
        json["usernameA"] = usernameA;
        json["usernameB"] = username;
        json["method"] = "chat";

        dbg("Converting JSON object to QByteArray");
        QJsonDocument jsonDoc(json);
        QByteArray jsonData = jsonDoc.toJson();

        dbg("Creating network request");
        QNetworkRequest request{QUrl(serverUrl)};
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        dbg("Sending POST request");
        networkManager->post(request, jsonData);

        messageInput->clear();
    } else {
        dbg("Message is empty, not sending.");
    }
}

void ChatWindow::handleNetworkReply(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObject = jsonResponse.object();

        bool success = jsonObject["success"].toBool();
        if (success) {
            dbg("Message sent successfully.");
            // Optionally, fetch the latest chat history
            getChatHistory();
        } else {
            dbg("Failed to send message.");
        }
    } else {
        chatDisplay->append("<font color=\"red\">Error: " + reply->errorString() + "</font>");
        dbg("Network error: " + reply->errorString());
    }

    reply->deleteLater();
}

void ChatWindow::getChatHistory() {
    dbg("Fetching chat history...");

    QJsonObject json;
    json["usernameA"] = usernameA;
    json["usernameB"] = username;
    json["method"] = "chathistory";

    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson();

    QNetworkRequest request{QUrl(serverUrl)};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    dbg("Sending POST request for chat history");
    QNetworkReply* reply = networkManager->post(request, jsonData);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() { handleHistoryReply(reply); });
}

void ChatWindow::handleHistoryReply(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonArray jsonArray = jsonResponse.array();

        messages.clear();
        for (const QJsonValue &value : jsonArray) {
            QJsonObject obj = value.toObject();
            QString message = obj["message"].toString();
            QString timeStr = obj["timestamp"].toString();
            QDateTime timestamp = QDateTime::fromString(timeStr, Qt::ISODate);
            messages.append(qMakePair(timestamp, message));
        }

        dbg("Sorting messages by timestamp");
        std::sort(messages.begin(), messages.end(), [](const QPair<QDateTime, QString> &a, const QPair<QDateTime, QString> &b) {
            return a.first < b.first;
        });

        dbg("Displaying messages");
        displayMessages();
    } else {
        chatDisplay->append("<font color=\"red\">Error: " + reply->errorString() + "</font>");
        dbg("Network error: " + reply->errorString());
    }

    reply->deleteLater();
}

void ChatWindow::clearChatHistory() {
    dbg("Clearing chat history...");

    QJsonObject json;
    json["usernameA"] = usernameA;
    json["usernameB"] = username;
    json["method"] = "clearhistory";

    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson();

    QNetworkRequest request{QUrl(serverUrl)};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    dbg("Sending POST request to clear chat history");
    QNetworkReply* reply = networkManager->post(request, jsonData);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() { handleClearHistoryReply(reply); });
}

void ChatWindow::handleClearHistoryReply(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObject = jsonResponse.object();

        bool success = jsonObject["success"].toBool();
        if (success) {
            dbg("Chat history cleared successfully.");
            messages.clear();
            displayMessages();
        } else {
            dbg("Failed to clear chat history.");
            chatDisplay->append("<font color=\"red\">Failed to clear chat history.</font>");
        }
    } else {
        chatDisplay->append("<font color=\"red\">Error: " + reply->errorString() + "</font>");
        dbg("Network error: " + reply->errorString());
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

void ChatWindow::startPolling() {
    dbg("Starting polling...");
    pollingTimer->start(5000); // Poll every 5 seconds
}

void ChatWindow::stopPolling() {
    dbg("Stopping polling...");
    pollingTimer->stop();
}
