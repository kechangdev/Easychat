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
#include <QTimer>
#include <QToolButton>

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

private slots:
    void sendMessage();
    void handleNetworkReply(QNetworkReply* reply);
    void getChatHistory();
    void handleHistoryReply(QNetworkReply* reply);
    void startPolling();
    void stopPolling();
    void clearChatHistory();
    void handleClearHistoryReply(QNetworkReply* reply);

private:
    void displayMessages();
    Ui::ChatWindow *ui;
    QString usernameA;
    QTextEdit *chatDisplay;
    QLineEdit *messageInput;
    QPushButton *sendButton;
    QToolButton *clearButton;
    QNetworkAccessManager *networkManager;
    const QString serverUrl = "https://chat.6b.fit";
    QString username;
    QVector<QPair<QDateTime, QString>> messages;
    Notification *notification;
    QTimer *pollingTimer;
};

#endif // CHATWINDOW_H
