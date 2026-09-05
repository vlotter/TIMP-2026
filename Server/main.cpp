#include <QCoreApplication>
#include "mytcpserver.h"
#include "database.h"
#include <QDebug>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    
    if (!DataBase::getInstance()->initDB()) {
        qDebug() << "Failed to initialize database!";
        return -1;
    }

    MyTcpServer server;
    return a.exec();
}
