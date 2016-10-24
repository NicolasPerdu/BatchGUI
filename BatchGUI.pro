#-------------------------------------------------
#
# Project created by QtCreator 2016-10-16T18:10:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BatchGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    droparea.cpp

HEADERS  += mainwindow.h \
    droparea.h \
    program.h \
    process.h

FORMS    +=

CONFIG	 += static
CONFIG   += console

RC_FILE = BatchGUI.rc
