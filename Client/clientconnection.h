#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QMutex>

class ClientConnection : public QObject {
    Q_OBJECT
public:
    static ClientConnection* getInstance();
    bool connectToServer(const QString& ip, int port);
    void disconnectFromServer();
    void sendRequest(const QByteArray& request);
    
    QString getRole() const { return currentRole; }
    void setRole(const QString& role) { currentRole = role; }

signals:
    void responseReceived(const QByteArray& response);

private slots:
    void onReadyRead();

private:
    explicit ClientConnection(QObject *parent = nullptr);
    ~ClientConnection();
    ClientConnection(const ClientConnection&) = delete;
    ClientConnection& operator=(const ClientConnection&) = delete;

    static ClientConnection* instance;
    static QMutex mutex;
    QTcpSocket* socket;
    QString currentRole;
};

#endif // CLIENTCONNECTION_H
