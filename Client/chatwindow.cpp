#include "chatwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ChatWindow), networkManager(new QNetworkAccessManager(this)), username(username), notification(new Notification)
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
    refreshButton = new QPushButton("Refresh", this); // Initialize refresh button

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(chatDisplay);
    layout->addWidget(messageInput);
    layout->addWidget(sendButton);
    layout->addWidget(clearButton);
    layout->addWidget(refreshButton); // Add refresh button to layout

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    dbg("Connecting signals and slots");
    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::sendMessage);
    connect(messageInput, &QLineEdit::returnPressed, this, &ChatWindow::sendMessage);
    connect(clearButton, &QToolButton::clicked, this, &ChatWindow::clearChatHistory);
    connect(refreshButton, &QPushButton::clicked, this, &ChatWindow::refreshChatHistory); // Connect refresh button

    dbg("Setting window title");
    setWindowTitle("Chat with " + username);

    dbg("Fetch chat history when the window is initialized");
    getChatHistory();
}

ChatWindow::~ChatWindow() {
    delete ui;
    delete notification;
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

        QJsonDocument jsonDoc(json);
        QByteArray jsonData = jsonDoc.toJson();
        dbg("JSON:" + jsonData);

        QNetworkRequest request{QUrl(serverUrl)};
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        dbg("Sending POST request");
        QNetworkReply* reply = networkManager->post(request, jsonData);
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            handleNetworkReply(reply);
        });

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

        dbg("Response: " + jsonResponse.toJson());
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
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleHistoryReply(reply);
    });
}

void ChatWindow::handleHistoryReply(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();

        dbg("Raw response data: " + QString(responseData));

        if (responseData.isEmpty()) {
            dbg("Response data is empty.");
            chatDisplay->append("<font color=\"red\">Error: Empty response from server</font>");
            reply->deleteLater();
            return;
        }

        QJsonParseError jsonError;
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData, &jsonError);

        if (jsonError.error != QJsonParseError::NoError) {
            dbg("JSON parse error: " + jsonError.errorString());
            chatDisplay->append("<font color=\"red\">Error: Failed to parse JSON response</font>");
            reply->deleteLater();
            return;
        }

        QJsonArray jsonArray = jsonResponse.array();
        dbg("JSON Array size: " + QString::number(jsonArray.size()));

        messages.clear();
        for (const QJsonValue &value : jsonArray) {
            QJsonObject obj = value.toObject();

            if (!obj.contains("message") || !obj.contains("usernameA")) {
                dbg("JSON object missing 'message' or 'usernameA' field: " + QString(QJsonDocument(obj).toJson(QJsonDocument::Compact)));
                continue;
            }

            QString message = obj["message"].toString();
            QString sender = obj["usernameA"].toString();
            QString timeStr = obj["timestamp"].toString();
            QDateTime timestamp = QDateTime::fromString(timeStr, Qt::ISODate);

            if (!timestamp.isValid()) {
                dbg("Invalid timestamp format: " + timeStr);
                continue;
            }

            dbg("Parsed message: " + message);
            dbg("Parsed sender: " + sender);
            dbg("Parsed timestamp: " + timestamp.toString());

            messages.append(qMakePair(timestamp, QString("[%1] %2: %3").arg(timestamp.toString("yyyy-MM-dd HH:mm:ss"), sender, message)));
        }

        dbg("Total messages parsed: " + QString::number(messages.size()));

        dbg("Sorting messages by timestamp");
        std::sort(messages.begin(), messages.end(), [](const QPair<QDateTime, QString> &a, const QPair<QDateTime, QString> &b) {
            return a.first < b.first;
        });

        dbg("Displaying messages");
        displayMessages();
        update(); // Ensure the widget updates immediately
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

    dbg("JSON:" + jsonData);

    QNetworkRequest request{QUrl(serverUrl)};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    dbg("Sending POST request to clear chat history");
    QNetworkReply* reply = networkManager->post(request, jsonData);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleClearHistoryReply(reply);
    });
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
        chatDisplay->append(message.second);
    }
}

void ChatWindow::refreshChatHistory() {
    dbg("Manually refreshing chat history");
    getChatHistory();
}
