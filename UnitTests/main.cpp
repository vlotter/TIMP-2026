#include <QtTest>
#include "functionsforserver.h"
#include "database.h"
#include <QString>

class TestFunctionsForServer : public QObject {
    Q_OBJECT

private slots:
    void testVigenere() {
        QString role = "guest";
        QByteArray req = "vigenere&HELLO&KEY";
        QByteArray res = FunctionsForServer::parseAndExecute(req, role);
        QVERIFY(res.startsWith("vigenere_res&"));
        QString ans = QString::fromUtf8(res).split("&")[1].trimmed();
        QCOMPARE(ans, QString("RIJVS"));
    }

    void testSha512() {
        QString role = "guest";
        QByteArray req = "sha512&test";
        QByteArray res = FunctionsForServer::parseAndExecute(req, role);
        QVERIFY(res.startsWith("sha512_res&"));
        QString ans = QString::fromUtf8(res).split("&")[1].trimmed();
        // hash of "test"
        QVERIFY(ans.startsWith("ee26b0dd4af7e749aa1a8ee3c10ae9923f618980772e473f8819a5d4940e0db2"));
    }

    void testBisection() {
        QString role = "guest";
        QByteArray req = "bisection&2&1&2"; // sqrt(2) is between 1 and 2
        QByteArray res = FunctionsForServer::parseAndExecute(req, role);
        QVERIFY(res.startsWith("bisection_res&"));
        double ans = QString::fromUtf8(res).split("&")[1].trimmed().toDouble();
        QVERIFY(std::abs(ans - 1.414) < 0.01);
    }
    
    void testGraph() {
        QString role = "guest";
        // start=0, end=2, V=3, 0-1 w=1, 1-2 w=2
        QByteArray req = "graph&0&2&3&0&1&1&1&2&2";
        QByteArray res = FunctionsForServer::parseAndExecute(req, role);
        QVERIFY(res.startsWith("graph_res&"));
        QString ans = QString::fromUtf8(res).split("&")[1].trimmed();
        QCOMPARE(ans, QString("3"));
    }
};

QTEST_MAIN(TestFunctionsForServer)
#include "main.moc"
