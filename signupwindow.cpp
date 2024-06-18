#include "signupwindow.h"
#include "ui_signupwindow.h"

SignupWindow::SignupWindow(QWidget *parent) : QDialog(parent), ui(new Ui::SignupWindow) {

    ui->setupUi(this);
}

SignupWindow::~SignupWindow()
{
    delete ui;
}
