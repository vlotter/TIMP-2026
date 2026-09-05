#include "mytcpserver.h"
#include "functionsforserver.h"
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
    mUserRoles[descriptor] = "guest"; // Роль по умолчанию

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
    
    QString role = mUserRoles[descriptor];
    QByteArray response = FunctionsForServer::parseAndExecute(array, role);
    mUserRoles[descriptor] = role; // Обновляем роль, если она изменилась при авторизации
    
    clientSocket->write(response);
}

void MyTcpServer::slotClientDisconnected() {
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    int descriptor = clientSocket->socketDescriptor();
    mTcpSockets.remove(descriptor);
    mUserRoles.remove(descriptor);
    clientSocket->deleteLater();
}
