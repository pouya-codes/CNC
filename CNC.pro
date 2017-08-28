#-------------------------------------------------
#
# Project created by QtCreator 2017-08-05T17:51:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv


TARGET = CNC
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        cameraview.cpp \
        clickablelabel.cpp

HEADERS  += mainwindow.h \
            asmOpenCV.h \
            cameraview.h \
            clickablelabel.h

FORMS    += mainwindow.ui \
    cameraview.ui
