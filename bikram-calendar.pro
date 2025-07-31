
QT += core gui widgets quick qml

SOURCES += \
    autostartmanager.cpp \
    main.cpp \
    tooltipmanager.cpp

RESOURCES += \
    resources.qrc
DEFINES += QML_SOURCES_DIR=\\\"$$PWD/qml\\\"
qml_files.files = $$PWD/qml
qml_files.path = qml

HEADERS += \
    autostartmanager.h \
    tooltipmanager.h
