#include "logindialog.h"
#include "ui_logindialog.h"
#include "clientconnection.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ClientConnection::getInstance(), &ClientConnection::responseReceived, this, &LoginDialog::onResponse);
}

LoginDialog::~LoginDialog() {
    disconnect(ClientConnection::getInstance(), &ClientConnection::responseReceived, this, &LoginDialog::onResponse);
    delete ui;
}

void LoginDialog::on_authBtn_clicked() {
    QString req = "auth&" + ui->loginEdit->text() + "&" + ui->passwordEdit->text();
    ClientConnection::getInstance()->sendRequest(req.toUtf8());
}

void LoginDialog::on_regBtn_clicked() {
    QString req = "reg&" + ui->loginEdit->text() + "&" + ui->passwordEdit->text();
    ClientConnection::getInstance()->sendRequest(req.toUtf8());
}

void LoginDialog::onResponse(const QByteArray& res) {
    QString resp = QString::fromUtf8(res).trimmed();
    if (resp.startsWith("auth_ok")) {
        QStringList parts = resp.split("&");
        if(parts.size() > 1) {
            ClientConnection::getInstance()->setRole(parts[1]);
        }
        accept();
    } else if (resp == "auth_fail") {
        QMessageBox::warning(this, "Error", "Invalid login or password");
    } else if (resp == "reg_ok") {
        QMessageBox::information(this, "Success", "Registration successful. Now you can login.");
    } else if (resp == "reg_fail") {
        QMessageBox::warning(this, "Error", "Registration failed");
    }
}
