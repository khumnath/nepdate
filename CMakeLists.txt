cmake_minimum_required(VERSION 3.16)
project(nepdate VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific settings
if (WIN32)
    message(STATUS "Configuring for Windows")
elseif (UNIX AND NOT APPLE)
    message(STATUS "Configuring for Linux")
endif()

# Find Qt6 package
find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core)

# Include directories (Qt6_INCLUDE_DIRS is not typically needed in modern CMake)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Set AUTOMOC, AUTORCC, and AUTOUIC to automatically generate necessary files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Helper function to add executables
function(add_nepdate_executable target_name sources headers forms resources)
    add_executable(${target_name} ${sources} ${headers} ${forms} ${resources})
    target_link_libraries(${target_name} PRIVATE Qt6::Widgets Qt6::Gui Qt6::Core)
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
