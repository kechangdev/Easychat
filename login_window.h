#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
    class login_window;
}
QT_END_NAMESPACE

class login_window : public QMainWindow {
    Q_OBJECT

public:
    login_window(QWidget *parent = nullptr);
    ~login_window();

private slots:
    void onLoginButtonClicked();

private:
    Ui::login_window *ui;
};
#endif // LOGIN_WINDOW_H
