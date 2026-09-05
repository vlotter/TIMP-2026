#include "clientconnection.h"

ClientConnection* ClientConnection::instance = nullptr;
QMutex ClientConnection::mutex;

ClientConnection* ClientConnection::getInstance() {
    QMutexLocker locker(&mutex);
    if (instance == nullptr) {
        instance = new ClientConnection();
    }
    return instance;
}

ClientConnection::ClientConnection(QObject *parent) : QObject(parent), currentRole("guest") {
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &ClientConnection::onReadyRead);
}

ClientConnection::~ClientConnection() {
    socket->close();
}

bool ClientConnection::connectToServer(const QString& ip, int port) {
    socket->connectToHost(ip, port);
    return socket->waitForConnected(3000);
}

void ClientConnection::disconnectFromServer() {
    socket->disconnectFromHost();
}

void ClientConnection::sendRequest(const QByteArray& request) {
    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->write(request);
        socket->flush();
    }
}

void ClientConnection::onReadyRead() {
    QByteArray data = socket->readAll();
    emit responseReceived(data);
}
