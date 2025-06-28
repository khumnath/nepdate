cmake_minimum_required(VERSION 3.16)

# Read version from VERSION.txt
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/resources/VERSION.txt" VERSION_CONTENTS)
string(STRIP "${VERSION_CONTENTS}" APP_VERSION)

# Project definition
project(nepdate VERSION ${APP_VERSION} LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific settings
if (WIN32)
    message(STATUS "Configuring for Windows")
elseif (UNIX AND NOT APPLE)
    message(STATUS "Configuring for Linux")
endif()

# Find Qt version
find_package(Qt6 QUIET COMPONENTS Widgets Gui Core)
if (NOT Qt6_FOUND)
    find_package(Qt5 QUIET COMPONENTS Widgets Gui Core)
    if (NOT Qt5_FOUND)
        message(FATAL_ERROR "Neither Qt6 nor Qt5 could be found")
    else()
        set(QT_VERSION_MAJOR 5)
        message(STATUS "Configuring Using Qt5")
    endif()
else()
    set(QT_VERSION_MAJOR 6)
    message(STATUS "Configuring Using Qt6")
endif()

# Set AUTOMOC, AUTORCC, and AUTOUIC to automatically generate necessary files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Helper function to add executables
function(add_nepdate_executable target_name sources headers forms resources)
    add_executable(${target_name} ${sources} ${headers} ${forms} ${resources})
    if (${QT_VERSION_MAJOR} EQUAL 6)
        target_link_libraries(${target_name} PRIVATE Qt6::Widgets Qt6::Gui Qt6::Core)
    elseif(${QT_VERSION_MAJOR} EQUAL 5)
        target_link_libraries(${target_name} PRIVATE Qt5::Widgets Qt5::Gui Qt5::Core)
    endif()
endfunction()


# Define nepdate-widget executable

set(NEPDATE_WIDGET_SOURCES
    main.cpp
    mainwindow.cpp
    calendarwindow.cpp
    calendarlogic.cpp
    calendartable.cpp
    converter.cpp
    panchanga.cpp
)

set(NEPDATE_WIDGET_HEADERS
    mainwindow.h
    bikram.h
    DayTithiWidget.h
    panchanga.h
    calendarwindow.h
    calendarlogic.h
    calendartable.h
    converter.h
)

set(NEPDATE_WIDGET_FORMS
    mainwindow.ui
    calendarwindow.ui
)

set(NEPDATE_WIDGET_RESOURCES
    resources.qrc
)


add_nepdate_executable(nepdate_widget "${NEPDATE_WIDGET_SOURCES}" "${NEPDATE_WIDGET_HEADERS}" "${NEPDATE_WIDGET_FORMS}" "${NEPDATE_WIDGET_RESOURCES}")


# Set the output name to nepdate-widget
set_target_properties(nepdate_widget PROPERTIES OUTPUT_NAME nepdate-widget)

# Define nepdate-calendar executable
set(NEPDATE_CALENDAR_SOURCES
    main_calendar.cpp
    calendarwindow.cpp
    mainwindow.cpp
    calendarlogic.cpp
    calendartable.cpp
    converter.cpp
    panchanga.cpp
)

set(NEPDATE_CALENDAR_HEADERS
    calendarwindow.h
    DayTithiWidget.h
    bikram.h
    mainwindow.h
    panchanga.h
    calendarlogic.h
    calendartable.h
    converter.h
)

set(NEPDATE_CALENDAR_FORMS
    calendarwindow.ui
    mainwindow.ui
)

set(NEPDATE_CALENDAR_RESOURCES
    resources.qrc
)

add_nepdate_executable(nepdate_calendar "${NEPDATE_CALENDAR_SOURCES}" "${NEPDATE_CALENDAR_HEADERS}" "${NEPDATE_CALENDAR_FORMS}" "${NEPDATE_CALENDAR_RESOURCES}")

# Set the output name to nepdate-calendar
set_target_properties(nepdate_calendar PROPERTIES OUTPUT_NAME nepdate-calendar)
