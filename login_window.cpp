#include "login_window.h"
#include "./ui_login_window.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

login_window::login_window(QWidget *parent) : QMainWindow(parent), ui(new Ui::login_window) {
    setFixedSize(651, 429);
    ui->setupUi(this);
    connect(ui->loginwindow_login_button, &QPushButton::clicked, this, &login_window::onLoginButtonClicked);
}

login_window::~login_window() {
    delete ui;
}

void login_window::onLoginButtonClicked() {
    qDebug() << "Login button clicked!";
    QString username = ui->loginwindow_username_input->text();
    QString password = ui->loginwindow_userpassword_input->text();
    qDebug() << "Username:" << username;
    qDebug() << "Password:" << password;
    qDebug() << "Try to login!";

    QJsonObject json;
    json["username"] = username;
    json["password"] = password;

    // 将 JSON 对象转换为字节数组
    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson();

    // 创建网络请求
    QNetworkRequest request(QUrl("http://yourserver.com/login"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 发送 POST 请求
    networkManager->post(request, jsonData);
}
