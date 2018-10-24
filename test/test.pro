#-------------------------------------------------
#
# Project created by QtCreator 2018-03-27T14:03:35
#
#-------------------------------------------------

QT       += core gui webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ./log

SOURCES += main.cpp\
        mainwindow.cpp \
    ztestaction.cpp \
    crequestaccessmanager.cpp \
    crequestaction.cpp \
    log/clogsetting.cpp

HEADERS  += mainwindow.h \
    ztestaction.h \
    crequestaccessmanager.h \
    crequestaction.h \
    log/cljlogcontent.h \
    log/cljlogdefines.h \
    log/clogsetting.h

FORMS    += mainwindow.ui