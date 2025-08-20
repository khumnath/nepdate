cmake_minimum_required(VERSION 3.16)

#=============================================================================
# Project Metadata and Versioning
#=============================================================================
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

#=============================================================================
# C++ Standard and Qt Setup
#=============================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Quick
    Qml
    PrintSupport
)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

#=============================================================================
# Run Versioning Script Before Build
#=============================================================================
execute_process(
    COMMAND bash "${CMAKE_CURRENT_SOURCE_DIR}/update_version.sh"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    RESULT_VARIABLE VERSION_SCRIPT_RESULT
)
if(NOT VERSION_SCRIPT_RESULT EQUAL 0)
    message(WARNING "The version update script failed to run. Version info may be stale or incorrect.")
endif()

#=============================================================================
# Source Files
#=============================================================================
set(SOURCES
    main.cpp
    autostartmanager.cpp
    tooltipmanager.cpp
    helper.cpp
)

set(HEADERS
    autostartmanager.h
    tooltipmanager.h
    helper.h
)

set(RESOURCES
    resources.qrc
)

qt_add_executable(bikram-calendar
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

target_link_libraries(bikram-calendar PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Quick
    Qt6::Qml
    Qt6::PrintSupport
)

#=============================================================================
# QML Folder Installation (for development)
#=============================================================================
#install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/qml"
#    DESTINATION "${CMAKE_INSTALL_PREFIX}/qml"
#   FILES_MATCHING PATTERN "*.qml"
#)

#=============================================================================
# Installation Rules
#=============================================================================
include(GNUInstallDirs)

if(NOT CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/.local")
endif()

message(STATUS "Install prefix set to: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Binary will be installed to: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}")

install(TARGETS bikram-calendar
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
