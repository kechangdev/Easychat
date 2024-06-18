#include "signupwindow.h"
#include "ui_signupwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>
#include <QDateTime>

SignupWindow::SignupWindow(QWidget *parent) : QDialog(parent), ui(new Ui::SignupWindow) {
    ui->setupUi(this);
}

SignupWindow::~SignupWindow() {
    delete ui;
}
