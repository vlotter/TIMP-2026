#ifndef CUSTOMSHA512_H
#define CUSTOMSHA512_H

#include <QString>
#include <QByteArray>

class CustomSha512 {
public:
    static QString hash(const QString& input);
};

#endif
