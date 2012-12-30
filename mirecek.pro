#-------------------------------------------------
#
# Project created by QtCreator 2012-12-30T11:40:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mirecek
TEMPLATE = app


SOURCES += main.cpp\
        harmonizer.cpp \
        hled.cpp

HEADERS  += harmonizer.h \
    hled.h \

FORMS    += harmonizer.ui

include(qextserialport/src/qextserialport.pri)
