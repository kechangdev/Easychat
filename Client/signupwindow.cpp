#include "signupwindow.h"
#include "ui_signupwindow.h"
#include "notification.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>
#include <QDateTime>

SignupWindow::SignupWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::SignupWindow), networkManager(new QNetworkAccessManager(this)), notification(new Notification) {
    dbg("Initializing signup window");
    ui->setupUi(this);
    dbg("Connecting signals and slots");
    connect(ui->Signup_buttonBox_Cancel_or_Ok, &QDialogButtonBox::accepted, this, &SignupWindow::onAccepted);
}

SignupWindow::~SignupWindow() {
    delete ui;
    delete notification;
}

void SignupWindow::onAccepted() {
    dbg("Accepted OK button was clicked");
    dbg("Getting user signup input...");
    QString username = ui->Signup_Username_input->text();
    QString password = ui->Signup_Password_input->text();
    QString confirmPassword = ui->Signup_Confirm_Password_input->text();

    ui->Signup_Username_input->clear();
    ui->Signup_Password_input->clear();
    ui->Signup_Confirm_Password_input->clear();

    if (password != confirmPassword) {
        dbg("Passwords do not match!");
        notification->display("Passwords do not match!");
        return;
    }

    dbg("Preparing request...");
    QJsonObject json;
    json["username"] = username;
    json["password"] = password;
    json["method"] = "signup";

    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson();

    dbg("Request prepared successfully!");

    QNetworkRequest request(QUrl(this->serverUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply* reply = networkManager->post(request, jsonData);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        dbg("Response received");
        QByteArray responseData = reply->readAll();
        dbg(responseData);
        if (signup_Response_Check(responseData)) {
            dbg("Signup successful");
            notification->display("Signup successful");
        } else {
            dbg("Signup failed");
            notification->display("Signup failed");
        }
        reply->deleteLater();
    });

    dbg("Request sent, waiting for response...");
}

bool SignupWindow::signup_Response_Check(const QByteArray &responseData) {
    dbg("Parsing JSON response");
    QJsonParseError parseError;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        dbg("JSON parse error: " + parseError.errorString());
        notification->display("JSON parse error: " + parseError.errorString());
        return false;
    }

    if (!jsonResponse.isObject()) {
        dbg("Invalid JSON response: not an object");
        notification->display("Invalid JSON response: not an object");
        return false;
    }

    QJsonObject responseObject = jsonResponse.object();

    if (!responseObject.contains("message") || !responseObject.contains("username")) {
        dbg("Invalid JSON response: missing required fields");
        notification->display("Invalid JSON response: missing required fields");
        return false;
    }

    QString message = responseObject["message"].toString();
    QString username = responseObject["username"].toString();

    dbg("Server response message: " + message);
    dbg("Username from response: " + username);

    return (message == "Signup successful");
}
