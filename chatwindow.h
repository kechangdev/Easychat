#ifndef CHATWINDOW_H
#define CHATWINDOW_H

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
    const QString serverUrl = "http://your-server-url.com";
    QString username;
    QVector<QPair<QDateTime, QString>> messages; // Stores messages with their timestamps
};

#endif // CHATWINDOW_H
