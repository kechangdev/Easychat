#include "login_window.h"
#include "./ui_login_window.h"
#include "notification.h"
#include "signupwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>
#include <QDateTime>

login_window::login_window(QWidget *parent) : QMainWindow(parent), ui(new Ui::login_window), networkManager(new QNetworkAccessManager(this)) {
    dbg("Programming...");

    dbg("init Login Window:");

    dbg("init Login Window Size");
    setFixedSize(651, 429);
    dbg("init Login Window UI");
    ui->setupUi(this);

    dbg("init Signup hyperlink");
    ui->signup_label->setText("<a href='#'>Don't have an account? Sign Up</a>");
    ui->signup_label->setTextFormat(Qt::RichText);
    ui->signup_label->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->signup_label->setOpenExternalLinks(false);

    dbg("init signal & slots:");
    dbg("init login button");
    connect(ui->loginwindow_login_button, &QPushButton::clicked, this, &login_window::onLoginButtonClicked);
    dbg("init network manager");
    connect(networkManager, &QNetworkAccessManager::finished, this, &login_window::onReplyFinished);
    dbg("init signup label");
    connect(ui->signup_label, &QLabel::linkActivated, this, &login_window::onSignupLinkClicked);

    dbg("init signup window");
    signupwindow = new SignupWindow(this);
}

login_window::~login_window() {
    delete signupwindow;
    delete ui;
}

void login_window::onSignupLinkClicked() {
    dbg("Show Signup Window");
    signupwindow->show();
}

void login_window::onLoginButtonClicked() {
    dbg("Login button clicked!");
    dbg("Trying to connect to Server...");
    QString username = ui->loginwindow_username_input->text();
    QString password = ui->loginwindow_userpassword_input->text();

    if (username.isEmpty() || password.isEmpty()) {
        dbg("Username or Password is empty.");
        Notification *notification = new Notification;
        notification->display("Username or Password is empty.");
        notification->show();
        return;
    }

    QUrl url(serverUrl + "/login");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = username;
    json["password"] = password;

    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson();

    networkManager->post(request, jsonData);
}

void login_window::onReplyFinished(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        dbg("Error in network reply: " + reply->errorString());
        return;
    }

    QByteArray responseData = reply->readAll();
    if (login_Response_Check(responseData)) dbg("Login successful!");
    else dbg("Login failed.");

    reply->deleteLater();
}

bool login_window::login_Response_Check(QByteArray& responseData) {
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    if (jsonResponse.isObject()) {
        QJsonObject jsonObject = jsonResponse.object();
        if (jsonObject.contains("success") && jsonObject["success"].toBool()) {
            return true;
        }
    }
    return false;
}
