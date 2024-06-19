#include "notification.h"
#include "./ui_notification.h"

Notification::Notification(QWidget *parent) : QWidget(parent), ui(new Ui::Notification) {
    ui->setupUi(this);
    setWindowModality(Qt::ApplicationModal);

    connect(ui->closeButton, &QPushButton::clicked, this, &Notification::onCloseButtonClicked);
}

Notification::~Notification() {
    delete ui;
}

void Notification::display(const QString &message) {
    ui->messageLabel->setText(message);
    ui->messageLabel->setAlignment(Qt::AlignCenter);
    show();
}

void Notification::onCloseButtonClicked() {
    close();
}
