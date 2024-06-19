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

QT_BEGIN_NAMESPACE
namespace Ui { class ChatWindow; }
QT_END_NAMESPACE

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    ChatWindow(const QString &username, QWidget *parent = nullptr);
    static void dbg(QString str) {
        qDebug() << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "] [INFO] " << str;
    }
    ~ChatWindow();

private slots:
    void sendMessage();
    void handleNetworkReply(QNetworkReply* reply);

private:
    void displayMessages();

    Ui::ChatWindow *ui;
    QTextEdit *chatDisplay;
    QLineEdit *messageInput;
    QPushButton *sendButton;
    QNetworkAccessManager *networkManager;
    const QString serverUrl = "https://chat.6b.fit";
    QString username;
    QVector<QPair<QDateTime, QString>> messages; // Stores messages with their timestamps
    Notification *notification = new Notification;
};

#endif // CHATWINDOW_H
