cmake_minimum_required(VERSION 3.16)

# Read version from file to set the project version
set(VERSION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/resources/version.conf")
if(EXISTS "${VERSION_FILE}")
    file(READ "${VERSION_FILE}" PROJECT_VERSION_STRING)
    string(STRIP "${PROJECT_VERSION_STRING}" PROJECT_VERSION_STRING)
else()
    set(PROJECT_VERSION_STRING "unknown")
    message(WARNING "Could not find version file: ${VERSION_FILE}. Defaulting to version ${PROJECT_VERSION_STRING}.")
endif()

project(bikram-calendar VERSION ${PROJECT_VERSION_STRING} LANGUAGES CXX)
message(STATUS "Configuring bikram-calendar version: ${PROJECT_VERSION}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS
    Core
    Gui
    Qml
    Quick
    Widgets
    QuickControls2
    PrintSupport
    REQUIRED)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_add_executable(bikram-calendar
    main.cpp
    autostartmanager.cpp
    tooltipmanager.cpp
    helper.cpp
    resources.qrc
)

target_sources(bikram-calendar PRIVATE
    autostartmanager.h
    tooltipmanager.h
    helper.h
)

target_link_libraries(bikram-calendar PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Widgets
    Qt6::QuickControls2
    Qt6::PrintSupport
)

#=============================================================================
# Installation Rules
#=============================================================================
# use:  cmake -DCMAKE_INSTALL_PREFIX=/path/to/your/custom/directory for install in your choice directory
# Include standard installation directories (e.g., bin, lib, include)
include(GNUInstallDirs)

if(NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/.local")
endif()

message(STATUS "Install prefix set to: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Binary will be installed to: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}")
install(TARGETS bikram-calendar
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
