#-------------------------------------------------
#
# Project created by QtCreator 2015-04-17T15:22:20
#
#-------------------------------------------------

QT+=sql
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SerialPort
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qextserialbase.cpp \
    qextserialport.cpp \
    win_qextserialport.cpp \
    dbserver.cpp

HEADERS  += mainwindow.h \
    qextserialbase.h \
    qextserialport.h \
    win_qextserialport.h \
    dbserver.h

FORMS    += mainwindow.ui
