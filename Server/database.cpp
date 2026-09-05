#include "database.h"
#include <QDebug>
#include <QCryptographicHash>

DataBase* DataBase::instance = nullptr;
QMutex DataBase::mutex;

DataBase* DataBase::getInstance() {
    QMutexLocker locker(&mutex);
    if (instance == nullptr) {
        instance = new DataBase();
    }
    return instance;
}

DataBase::DataBase(QObject *parent) : QObject(parent) {
}

DataBase::~DataBase() {
    if (db.isOpen()) {
        db.close();
    }
}

bool DataBase::initDB() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("server_db.sqlite");

    if (!db.open()) {
        qDebug() << "Failed to open database" << db.lastError().text();
        return false;
    }

    QSqlQuery query(db);
    QString createTableQuery = "CREATE TABLE IF NOT EXISTS users ("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                               "login TEXT UNIQUE NOT NULL, "
                               "password TEXT NOT NULL, "
                               "role TEXT DEFAULT 'user')";
    if (!query.exec(createTableQuery)) {
        qDebug() << "Failed to create table" << query.lastError().text();
        return false;
    }

    // Создание администратора по умолчанию
    query.prepare("SELECT COUNT(*) FROM users WHERE login = 'admin'");
    if (query.exec() && query.next() && query.value(0).toInt() == 0) {
        registerUser("admin", "admin");
        setRole("admin", "admin");
    }

    return true;
}

bool DataBase::registerUser(const QString& login, const QString& password) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO users (login, password, role) VALUES (:login, :password, 'user')");
    query.bindValue(":login", login);

    QByteArray pwdHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    query.bindValue(":password", pwdHash.toHex());

    if (!query.exec()) {
        qDebug() << "Register error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DataBase::authUser(const QString& login, const QString& password, QString& role) {
    QSqlQuery query(db);
    query.prepare("SELECT password, role FROM users WHERE login = :login");
    query.bindValue(":login", login);

    if (query.exec() && query.next()) {
        QString storedHash = query.value(0).toString();
        role = query.value(1).toString();
        QByteArray pwdHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
        return (storedHash == pwdHash.toHex());
    }
    return false;
}

bool DataBase::getUserRole(const QString& login, QString& role) {
    QSqlQuery query(db);
    query.prepare("SELECT role FROM users WHERE login = :login");
    query.bindValue(":login", login);

    if (query.exec() && query.next()) {
        role = query.value(0).toString();
        return true;
    }
    return false;
}

bool DataBase::setRole(const QString& login, const QString& role) {
    QSqlQuery query(db);
    query.prepare("UPDATE users SET role = :role WHERE login = :login");
    query.bindValue(":role", role);
    query.bindValue(":login", login);
    return query.exec();
}
