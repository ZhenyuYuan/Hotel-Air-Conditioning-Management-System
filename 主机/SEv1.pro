#-------------------------------------------------
#
# Project created by QtCreator 2017-04-08T15:31:40
#
#-------------------------------------------------

QT       += core gui
QT += network
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SEv1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    setting.cpp \
    slave.cpp \
    slavelist.cpp \
    mainframe.cpp \
    server.cpp \
    tcpclientsocket.cpp \
    database.cpp \
    statementdialog.cpp \
    statementitem.cpp \
    reportdialog.cpp

HEADERS  += mainwindow.h \
    setting.h \
    format.h \
    slave.h \
    slavelist.h \
    mainframe.h \
    server.h \
    tcpclientsocket.h \
    statementdialog.h \
    statementitem.h \
    reportdialog.h

FORMS    += mainwindow.ui \
    setting.ui \
    statemenItem.ui \
    statementdialog.ui \
    reportdialog.ui

DISTFILES +=
