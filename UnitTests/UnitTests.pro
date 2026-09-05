QT += core testlib network sql
CONFIG += c++17 console
TARGET = UnitTests
TEMPLATE = app

LIBS += -lcrypto -lssl
INCLUDEPATH += ../Server

SOURCES += main.cpp \
           ../Server/functionsforserver.cpp \
           ../Server/database.cpp \
           ../Server/customsha512.cpp

HEADERS += ../Server/functionsforserver.h \
           ../Server/database.h \
           ../Server/customsha512.h
