#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVariant>
#include <QMutex>

class DataBase : public QObject {
    Q_OBJECT
public:
    static DataBase* getInstance();
    
    bool initDB();
    bool registerUser(const QString& login, const QString& password);
    bool authUser(const QString& login, const QString& password, QString& role);
    bool getUserRole(const QString& login, QString& role);
    bool setRole(const QString& login, const QString& role);
    
private:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();
    DataBase(const DataBase&) = delete;
    DataBase& operator=(const DataBase&) = delete;

    static DataBase* instance;
    static QMutex mutex;
    QSqlDatabase db;
};

#endif
