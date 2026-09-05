QT += core network sql
CONFIG += c++17
TARGET = Server
TEMPLATE = app

LIBS += -lcrypto -lssl

SOURCES += main.cpp \
           mytcpserver.cpp \
           functionsforserver.cpp \
           database.cpp \
           customsha512.cpp

HEADERS += mytcpserver.h \
           functionsforserver.h \
           database.h \
           customsha512.h
