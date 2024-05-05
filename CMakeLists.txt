cmake_minimum_required(VERSION 3.16)
project(nepdate VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui)

# Include Qt6 include directories
include_directories(${Qt6_INCLUDE_DIRS})

# Set AUTOMOC, AUTORCC, and AUTOUIC to automatically generate necessary files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Add your source files
set(SOURCES
    main.cpp
    calendarwindow.cpp
    mainwindow.cpp
)

set(HEADERS
    mainwindow.h
    bikram.h
    calendarwindow.h
)

set(FORMS
    mainwindow.ui
    calendarwindow.ui
)

# Create the executable
add_executable(nepdate ${SOURCES} ${HEADERS} ${FORMS})

# Link against Qt6 libraries
target_link_libraries(nepdate PRIVATE Qt6::Widgets Qt6::Gui)
