#-------------------------------------------------
#
# Project created by QtCreator 2015-12-11T21:23:11
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -std=gnu++0x
QT       += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Query
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    BKTree.h \
    BKNode.h \
    TSTNode.h \
    TST.h

FORMS    += mainwindow.ui

