
#-------------------------------------------------
#
# Project created by QtCreator 2024-04-27 T11:24:12
#
#-------------------------------------------------

QT += core gui widgets

# Define the main executable
TEMPLATE = app
 TARGET = nepdate-calendar
 SOURCES = main_calendar.cpp \
           calendarwindow.cpp \
           mainwindow.cpp

 HEADERS = calendarwindow.h \
           bikram.h \
           mainwindow.h

 FORMS = calendarwindow.ui \
         mainwindow.ui

    # Resources specific to nepdate-calendar
 RESOURCES += resources.qrc

# Enable warning about missing libraries
CONFIG += warn_on
