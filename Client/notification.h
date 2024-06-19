#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QWidget>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class Notification;
}
QT_END_NAMESPACE

class Notification : public QWidget {
    Q_OBJECT

public:
    explicit Notification(QWidget *parent = nullptr);
    void display(const QString &message);
    ~Notification();

private slots:
    void onCloseButtonClicked();

private:
    Ui::Notification *ui;
};

#endif // NOTIFICATION_H
