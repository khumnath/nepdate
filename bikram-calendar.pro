#=============================================================================
#bikram-calendar desktop widget and calendar
#=============================================================================
# Executable
TARGET = bikram-calendar

# C++ standard
CONFIG += c++17

#=============================================================================
# Qt Modules
#=============================================================================
QT += core gui widgets quick qml printsupport

#=============================================================================
# Project Files
#=============================================================================

SOURCES += \
    main.cpp \
    autostartmanager.cpp \
    tooltipmanager.cpp \
    helper.cpp

HEADERS += \
    autostartmanager.h \
    tooltipmanager.h \
    helper.h

RESOURCES += \
    resources.qrc

#=============================================================================
# Set Qml Dir for devlopment(this copies qml folder along with executable path
# when run make install
#=============================================================================

# DEFINES += QML_SOURCES_DIR=\\\"$$PWD/qml\\\"
# qml_files.files = $$PWD/qml
# qml_files.path = qml

