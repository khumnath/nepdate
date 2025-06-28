#-------------------------------------------------
#
# Project created by QtCreator 2024-04-27 T11:24:12
#
#-------------------------------------------------
QT       += core gui widgets

TARGET = nepdate-widget
TEMPLATE = app

SOURCES += main.cpp \
           calendarwindow.cpp \
           mainwindow.cpp

HEADERS += mainwindow.h \
           bikram.h \
           panchanga.h \
           DayTithiWidget.h \
           calendarwindow.h

FORMS += mainwindow.ui \
         calendarwindow.ui
RESOURCES += \
            resources.qrc

DISTFILES += \
    resources/Laila-Regular.ttf
