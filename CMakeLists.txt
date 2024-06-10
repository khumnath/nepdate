cmake_minimum_required(VERSION 3.16)
project(nepdate VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 package
find_package(Qt6 REQUIRED COMPONENTS Widgets Gui)

# Include directories
include_directories(${Qt6_INCLUDE_DIRS})

# Set AUTOMOC, AUTORCC, and AUTOUIC to automatically generate necessary files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

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
add_executable(nepdate ${NEPDATE_SOURCES} ${NEPDATE_HEADERS} ${NEPDATE_FORMS} ${NEPDATE_RESOURCES})

# Link against Qt6 libraries
target_link_libraries(nepdate PRIVATE Qt6::Widgets Qt6::Gui)

### nepdate-calendar executable ###

# Collect nepdate-calendar sources
set(NEPDATE_CALENDAR_SOURCES
    main_calendar.cpp
    calendarwindow.cpp
)

# Collect nepdate-calendar headers
set(NEPDATE_CALENDAR_HEADERS
    calendarwindow.h
)

# Collect nepdate-calendar forms
set(NEPDATE_CALENDAR_FORMS
    calendarwindow.ui
)

# Collect nepdate-calendar resources
set(NEPDATE_CALENDAR_RESOURCES
    resources.qrc
)

# Create nepdate-calendar executable
add_executable(nepdate-calendar ${NEPDATE_CALENDAR_SOURCES} ${NEPDATE_CALENDAR_HEADERS} ${NEPDATE_CALENDAR_FORMS} ${NEPDATE_CALENDAR_RESOURCES})

# Link against Qt6 libraries
target_link_libraries(nepdate-calendar PRIVATE Qt6::Widgets Qt6::Gui)
