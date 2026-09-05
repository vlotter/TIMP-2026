#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_authBtn_clicked();
    void on_regBtn_clicked();
    void onResponse(const QByteArray& res);

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
