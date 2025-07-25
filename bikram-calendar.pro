QT += quick qml

SOURCES += \
    autostartmanager.cpp \
    main.cpp

RESOURCES += \
    resources.qrc
DEFINES += QML_SOURCES_DIR=\\\"$$PWD/qml\\\"
qml_files.files = $$PWD/qml
qml_files.path = qml

HEADERS += \
    autostartmanager.h
