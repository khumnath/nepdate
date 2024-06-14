#-------------------------------------------------
#
# Project created by QtCreator 2024-04-27 T11:24:12
#
#-------------------------------------------------

QT += core gui widgets

# Define the main executable
TEMPLATE = app
TARGET = nepdate-calendar
SOURCES += main_calendar.cpp \
           calendarwindow.cpp \
           mainwindow.cpp

HEADERS += calendarwindow.h \
           DayTithiWidget.h \
           bikram.h \
           mainwindow.h \
           panchanga.h

FORMS += calendarwindow.ui \
         mainwindow.ui

# Resources specific to nepdate-calendar
RESOURCES += resources.qrc

# Enable warning about missing libraries
CONFIG += warn_on

# Compiler flags
QMAKE_CXXFLAGS += -std=c++17

# Windows-specific settings
win32 {
    # Specify the path to the Qt installation. change here if you have different Qt version
    QMAKE_LIBDIR += C:/Qt/6.6.1/mingw_64/lib
    QMAKE_INCDIR += C:/Qt/6.6.1/mingw_64/include

    # Add other Windows-specific configurations here
    CONFIG += windows
    QMAKE_LFLAGS += -static

    # Prevent linker errors with MinGW
    QMAKE_LFLAGS_WINDOWS += -static-libgcc -static-libstdc++
}

# Linux-specific settings
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += Qt6Widgets Qt6Gui Qt6Core

    # Ensure dynamic linking on Linux
    QMAKE_LFLAGS -= -static
}
