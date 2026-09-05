#include <QCoreApplication>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("Test.db");

    if (!db.open()) {
        qDebug() << db.lastError().text();
        return 1;
    }

    QSqlQuery query(db);

    query.exec("CREATE TABLE IF NOT EXISTS User("
               "login    VARCHAR(20) NOT NULL, "
               "password VARCHAR(20) NOT NULL"
               ")");

    auto insertUser = [&](const QString& login, const QString& password) {
        query.prepare("INSERT INTO User(login, password) "
                      "VALUES (:login, :password)");
        query.bindValue(":login",    login);
        query.bindValue(":password", password);
        if (!query.exec())
            qDebug() << "Insert error:" << query.lastError().text();
    };

    insertUser("admin", "123");
    insertUser("user",  "qwerty");

    query.exec("SELECT * FROM User");

    QSqlRecord rec              = query.record();
    const int  loginIndex       = rec.indexOf("login");
    const int  passwordIndex    = rec.indexOf("password");

    qDebug() << "=== Users ===";
    while (query.next()) {
        qDebug() << query.value(loginIndex).toString()
                 << "\t" << query.value(passwordIndex).toString();
    }

    db.close();
    return 0;
}
