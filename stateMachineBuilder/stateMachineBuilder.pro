#-------------------------------------------------
#
# Project created by QtCreator 2018-11-05T08:51:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = stateMachineBuilder
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    parser.cpp \
    stateobj.cpp \
    drawarea.cpp \
    arrow.cpp

HEADERS  += mainwindow.h \
    parser.h \
    stateobj.h \
    drawarea.h \
    enums.h \
    arrow.h

FORMS    += mainwindow.ui
