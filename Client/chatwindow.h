#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include "notification.h"
#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVector>
#include <QPair>
#include <QDateTime>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui { class ChatWindow; }
QT_END_NAMESPACE

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    ChatWindow(const QString &username, QWidget *parent = nullptr);
    void getUsername(QString& _usernameA) { usernameA = _usernameA; }
    static void dbg(const QString &str) {
        qDebug() << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "] [INFO] " << str;
    }
    ~ChatWindow();

public slots:
    void sendMessage();
    void handleNetworkReply(QNetworkReply* reply);
    void getChatHistory();
    void handleHistoryReply(QNetworkReply* reply);
    void clearChatHistory();
    void handleClearHistoryReply(QNetworkReply* reply);
    void refreshChatHistory();

private:
    void displayMessages();
    Ui::ChatWindow *ui;
    QString usernameA;
    QTextEdit *chatDisplay;
    QLineEdit *messageInput;
    QPushButton *sendButton;
    QToolButton *clearButton;
    QPushButton *refreshButton; // Added for manual refresh
    QNetworkAccessManager *networkManager;
    const QString serverUrl = "http://49.234.177.172:10086/api";
    QString username;
    QVector<QPair<QDateTime, QString>> messages;
    Notification *notification;
};

#endif // CHATWINDOW_H
