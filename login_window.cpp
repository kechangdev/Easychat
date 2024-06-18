#include "login_window.h"
#include "./ui_login_window.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>
#include <QDateTime>

login_window::login_window(QWidget *parent) : QMainWindow(parent), ui(new Ui::login_window), networkManager(new QNetworkAccessManager(this)) {
    setFixedSize(651, 429);
    ui->setupUi(this);
    connect(ui->loginwindow_login_button, &QPushButton::clicked, this, &login_window::onLoginButtonClicked);
    connect(networkManager, &QNetworkAccessManager::finished, this, &login_window::onReplyFinished);

    ui->signup_link = new QLabel("Don't have an account? <a href='#'>Sign Up</a>", this);
    ui->signup_link->setTextFormat(Qt::RichText);
    ui->signup_link->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->signup_link->setOpenExternalLinks(false);
    connect(ui->signup_link, &QLabel::linkActivated, this, &::login_window::onSignupLinkClicked);
}

login_window::~login_window() {
    delete ui;
}

void login_window::dbg(QString str) {
    qDebug() << "[" << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "] [INFO] " << str;
}
void login_window::onSignupLinkClicked(const QString &link) {
    dbg("Show Signup Window");
    if (!signupwindow) signupwindow = new SignupWindow(this);
    signupwindow->show();
}

void login_window::onLoginButtonClicked() {
    dbg("Login button clicked!");
    dbg("Trying to connect to Server:");

    dbg("Trying to get user input...");
    QString username = ui->loginwindow_username_input->text();
    QString password = ui->loginwindow_userpassword_input->text();
    dbg("Username:" + username);
    dbg("Password:" + password);
    dbg("Method: login");

    dbg("Translating Password & Username to JSON...");
    QJsonObject json;
    json["username"] = username;
    json["password"] = password;
    json["method"] = "login";
    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson();
    dbg("Translation Success!");

    dbg("Creating network request...");
    QNetworkRequest request(QUrl(this->Server_URL));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    dbg("Network request created successfully!");

    dbg("Sending POST request...");
    networkManager->post(request, jsonData);

    dbg("All done, waiting for response...");
}

bool login_window::login_Response_Check(QByteArray& responseData) {
    dbg("Parsing JSON response:");
    QJsonParseError parseError;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        dbg("JSON parse error: " + parseError.errorString());
        return 0;
    }

    if (!jsonResponse.isObject()) {
        dbg("Invalid JSON response: not an object");
        return 0;
    }

    QJsonObject responseObject = jsonResponse.object();

    if (!responseObject.contains("message") || !responseObject.contains("username")) {
        dbg("Invalid JSON response: missing required fields");
        return 0;
    }

    QString message = responseObject["message"].toString();
    QString username = responseObject["username"].toString();

    dbg("Server response message:" + message);
    dbg("Username from response:" + username);

    return (message == "Login successful");
}

void login_window::onReplyFinished(QNetworkReply *reply) {
    dbg("Server responded!");
    dbg("Analyzing response:");
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        dbg("Response received:" + responseData);
        if (login_Response_Check(responseData)) {
            dbg("Login successful!");
            //
            //
            //
            //
        }
    } else {
        dbg("Error:" + reply->errorString());
        dbg("Login failed!");
    }
    reply->deleteLater();
}
