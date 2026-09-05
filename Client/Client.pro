QT += core gui widgets network
CONFIG += c++17
TARGET = Client
TEMPLATE = app

SOURCES += main.cpp \
           clientconnection.cpp \
           mainwindow.cpp \
           logindialog.cpp

HEADERS += clientconnection.h \
           mainwindow.h \
           logindialog.h

FORMS += mainwindow.ui \
         logindialog.ui
