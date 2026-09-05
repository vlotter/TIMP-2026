#include "mytcpserver.h"
#include "functionsforserver.h"
#include "database.h"
#include <QDebug>
#include <QCoreApplication>

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent) {
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    if (!mTcpServer->listen(QHostAddress::Any, 33333)) {
        qDebug() << "Server is not started";
    } else {
        qDebug() << "Server is started on port 33333";
    }
}

MyTcpServer::~MyTcpServer() {
    mTcpServer->close();
}

void MyTcpServer::slotNewConnection() {
    QTcpSocket* clientSocket = mTcpServer->nextPendingConnection();
    int descriptor = clientSocket->socketDescriptor();
    mTcpSockets[descriptor] = clientSocket;
    mUserLogins[descriptor] = ""; // Логин по умолчанию пуст

    connect(clientSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);
}

void MyTcpServer::slotServerRead() {
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    int descriptor = clientSocket->socketDescriptor();
    
    QByteArray array;
    while(clientSocket->bytesAvailable() > 0) {
        array.append(clientSocket->readAll());
    }
    
    QString currentLogin = mUserLogins[descriptor];
    QString currentRole = "guest";
    
    if (!currentLogin.isEmpty()) {
        DataBase::getInstance()->getUserRole(currentLogin, currentRole);
    }
    
    QByteArray response = FunctionsForServer::parseAndExecute(array, currentRole);
    
    // Если это была успешная авторизация, запоминаем логин в сессии
    QString reqStr = QString::fromUtf8(array).trimmed();
    QStringList parts = reqStr.split("&");
    if (parts.size() >= 3 && parts[0] == "auth" && response.startsWith("auth_ok")) {
        mUserLogins[descriptor] = parts[1];
    }
    
    clientSocket->write(response);
}

void MyTcpServer::slotClientDisconnected() {
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    int descriptor = clientSocket->socketDescriptor();
    mTcpSockets.remove(descriptor);
    mUserLogins.remove(descriptor);
    clientSocket->deleteLater();
}
