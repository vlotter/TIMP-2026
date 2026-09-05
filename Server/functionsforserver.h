#ifndef FUNCTIONSFORSERVER_H
#define FUNCTIONSFORSERVER_H

#include <QString>
#include <QByteArray>

class FunctionsForServer {
public:
    static QByteArray parseAndExecute(const QByteArray& request, QString& currentUserRole);

private:
    static QByteArray handleAuth(const QStringList& parts, QString& role);
    static QByteArray handleReg(const QStringList& parts);
    static QByteArray handleVigenere(const QStringList& parts);
    static QByteArray handleVigenereDec(const QStringList& parts);
    static QByteArray handleSha512(const QStringList& parts);
    static QByteArray handleBisection(const QStringList& parts);
    static QByteArray handleGraph(const QStringList& parts);
    static QByteArray handleAdmin(const QStringList& parts, const QString& role);
};

#endif // FUNCTIONSFORSERVER_H
