#include "mainwindow.h"
#include "logindialog.h"
#include "clientconnection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    if (!ClientConnection::getInstance()->connectToServer("127.0.0.1", 33333)) {
        QMessageBox::critical(nullptr, "Error", "Cannot connect to server");
        return -1;
    }

    LoginDialog login;
    if (login.exec() == QDialog::Accepted) {
        MainWindow w;
        w.show();
        return a.exec();
    }
    
    return 0;
}
