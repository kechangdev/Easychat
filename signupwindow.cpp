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
    : QDialog(parent),
    ui(new Ui::SignupWindow),
    networkManager(new QNetworkAccessManager(this)) {

    ui->setupUi(this);

    connect(ui->Signup_buttonBox_Cancel_or_Ok, &QDialogButtonBox::accepted, this, &SignupWindow::onAccepted);
}

SignupWindow::~SignupWindow() {
    delete ui;
}

void SignupWindow::onAccepted() {
    dbg("Accepted OK button was clicked");
    dbg("Trying to get user signup input...");
    QString username = ui->Signup_Username_input->text();
    QString password = ui->Signup_Password_input->text();
    QString confirm_password = ui->Signup_Confirm_Password_input->text();

    if (password != confirm_password) {
        dbg("Password != confirm_password!");
        return;
    }

    dbg("Username:" + username);
    dbg("Password:" + password);
    dbg("Method: Signup");
    dbg("Translating Password & Username to JSON...");

    QJsonObject json;
    json["username"] = username;
    json["password"] = password;
    json["method"] = "signup";

    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson();

    dbg("Translation Success!");

    dbg("Creating network request...");
    QNetworkRequest request(QUrl(this->serverUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    dbg("Network request created successfully!");

    dbg("Sending POST request...");
    QNetworkReply* reply = networkManager->post(request, jsonData);

    connect(reply, &QNetworkReply::finished, [=]() {
        dbg("Response received.");
        QByteArray responseData = reply->readAll();
        if (signup_Response_Check(responseData)) {
            dbg("Signup successful.");
        } else {
            dbg("Signup failed.");
        }
        reply->deleteLater();
    });

    dbg("All done, waiting for response...");
}

bool SignupWindow::signup_Response_Check(QByteArray& responseData) {
    dbg("Parsing JSON response:");
    QJsonParseError parseError;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        dbg("JSON parse error: " + parseError.errorString());
        return false;
    }

    if (!jsonResponse.isObject()) {
        dbg("Invalid JSON response: not an object");
        return false;
    }

    QJsonObject responseObject = jsonResponse.object();

    if (!responseObject.contains("message") || !responseObject.contains("username")) {
        dbg("Invalid JSON response: missing required fields");
        return false;
    }

    QString message = responseObject["message"].toString();
    QString username = responseObject["username"].toString();

    dbg("Server response message:" + message);
    dbg("Username from response:" + username);

    return (message == "Signup successful");
}
