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
           mainwindow.cpp \
           calendarlogic.cpp \
           calendartable.cpp \
           converter.cpp \
           panchanga.cpp

HEADERS += mainwindow.h \
           calendarwindow.h \
           calendarlogic.h \
           calendartable.h \
           converter.h \
           bikram.h \
           DayTithiWidget.h \
           panchanga.h

FORMS += mainwindow.ui \
         calendarwindow.ui
RESOURCES += \
            resources.qrc
