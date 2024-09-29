cmake_minimum_required(VERSION 3.16)
project(nepdate VERSION 2.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific settings
if (WIN32)
    message(STATUS "Configuring for Windows")
elseif (UNIX AND NOT APPLE)
    message(STATUS "Configuring for Linux")
endif()

# Try to find the highest available version of Qt (Qt7, Qt6, or Qt5)
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

### nepdate executable ###

# Collect nepdate sources
set(NEPDATE_SOURCES
    main.cpp
    mainwindow.cpp
    calendarwindow.cpp
)

# Collect nepdate headers
set(NEPDATE_HEADERS
    mainwindow.h
    bikram.h
    DayTithiWidget.h
    panchanga.h
    calendarwindow.h
)

# Collect nepdate forms
set(NEPDATE_FORMS
    mainwindow.ui
    calendarwindow.ui
)

# Collect nepdate resources
set(NEPDATE_RESOURCES
    resources.qrc
)

# Create nepdate executable
add_nepdate_executable(nepdate-widget "${NEPDATE_SOURCES}" "${NEPDATE_HEADERS}" "${NEPDATE_FORMS}" "${NEPDATE_RESOURCES}")

### nepdate-calendar executable ###

# Collect nepdate-calendar sources
set(NEPDATE_CALENDAR_SOURCES
    main.cpp
    mainwindow.cpp
    calendarwindow.cpp
    mainwindow.cpp
)

# Collect nepdate-calendar headers
set(NEPDATE_CALENDAR_HEADERS
    calendarwindow.h
    DayTithiWidget.h
    bikram.h
    panchanga.h
    mainwindow.h
)

# Collect nepdate-calendar forms
set(NEPDATE_CALENDAR_FORMS
    calendarwindow.ui
    mainwindow.ui
)

# Collect nepdate-calendar resources
set(NEPDATE_CALENDAR_RESOURCES
    resources.qrc
)

# Create nepdate-calendar executable
add_nepdate_executable(nepdate-calendar "${NEPDATE_CALENDAR_SOURCES}" "${NEPDATE_CALENDAR_HEADERS}" "${NEPDATE_CALENDAR_FORMS}" "${NEPDATE_CALENDAR_RESOURCES}")
