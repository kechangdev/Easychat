#include "login_window.h"
#include "./ui_login_window.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>
#include <QDateTime>

login_window::login_window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::login_window), signupwindow(new SignupWindow(this)),
    networkManager(new QNetworkAccessManager(this)), notification(new Notification),
    userlist(new UserList) {
    dbg("Programming...");

    dbg("init Login Window:");
    setFixedSize(651, 429);
    ui->setupUi(this);

    dbg("init Signup hyperlink");
    ui->signup_label->setText("<a href='#'>Don't have an account? Sign Up</a>");
    ui->signup_label->setTextFormat(Qt::RichText);
    ui->signup_label->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->signup_label->setOpenExternalLinks(false);

    dbg("init signal & slots:");
    connect(ui->loginwindow_login_button, &QPushButton::clicked, this, &login_window::onLoginButtonClicked);
    connect(networkManager, &QNetworkAccessManager::finished, this, &login_window::onReplyFinished);
    connect(ui->signup_label, &QLabel::linkActivated, this, &login_window::onSignupLinkClicked);
}

login_window::~login_window() {
    delete signupwindow;
    delete ui;
    delete notification;
    delete userlist;
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
    QString method = "login";

    usernameA = username;

    if (username.isEmpty() || password.isEmpty()) {
        dbg("Username or Password is empty.");
        notification->display("Username or Password is empty.");
        ui->loginwindow_username_input->clear();
        ui->loginwindow_userpassword_input->clear();
        return;
    }

    dbg("Constructing request:");
    dbg("Target host: " + serverUrl);
    QUrl url(serverUrl);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["username"] = username;
    json["password"] = password;
    json["method"] = method;
    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson();

    networkManager->post(request, jsonData);
}

void login_window::onReplyFinished(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        dbg("Error in network reply: " + reply->errorString());
        notification->display("Error in network reply: " + reply->errorString());
        ui->loginwindow_username_input->clear();
        ui->loginwindow_userpassword_input->clear();
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    if (loginResponseCheck(responseData)) {
        dbg("Login successful!");
        hide();
        userlist->getUsername(usernameA);
        userlist->show();
    } else {
        notification->display("Login failed.");
        dbg("Login failed.");
    }
    ui->loginwindow_username_input->clear();
    ui->loginwindow_userpassword_input->clear();
    reply->deleteLater();
}

bool login_window::loginResponseCheck(const QByteArray &responseData) {
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
    if (jsonResponse.isObject()) {
        QJsonObject jsonObject = jsonResponse.object();
        dbg("Check the Status from response...");
        return jsonObject.contains("Status") && jsonObject["Status"].toString() == "Login Successful";
    }
    return false;
}
