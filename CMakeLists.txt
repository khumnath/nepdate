cmake_minimum_required(VERSION 3.16)

#=============================================================================
# Project Metadata and Versioning
#=============================================================================

execute_process(
    COMMAND bash "${CMAKE_CURRENT_SOURCE_DIR}/update_version.sh"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    RESULT_VARIABLE VERSION_SCRIPT_RESULT
)
if(NOT VERSION_SCRIPT_RESULT EQUAL 0)
    message(WARNING "The version update script failed to run. Version info may be stale or incorrect.")
endif()

set(VERSION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/resources/version.conf")

if(EXISTS "${VERSION_FILE}")
    file(READ "${VERSION_FILE}" PROJECT_VERSION_STRING)
    string(STRIP "${PROJECT_VERSION_STRING}" PROJECT_VERSION_STRING)
else()
    set(PROJECT_VERSION_STRING "0.0.0")
    message(WARNING "Could not find version file: ${VERSION_FILE}. Defaulting to version ${PROJECT_VERSION_STRING}.")
endif()

project(bikram-calendar VERSION ${PROJECT_VERSION_STRING} LANGUAGES C CXX)
message(STATUS "Configuring bikram-calendar version: ${PROJECT_VERSION_STRING}")

#=============================================================================
# C++ Standard and Qt Setup
#=============================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
# Source Files
#=============================================================================
set(SOURCES
    main.cpp
    autostartmanager.cpp
    tooltipmanager.cpp
    helper.cpp
    src/astronomy.c
    src/panchangacalculator.cpp
    src/bikram.cpp
    src/bsmonthdata.cpp
    src/eventsdata.cpp
    src/constants.cpp
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

enable_testing()

qt_add_executable(run_test
    tests/test.cpp
    src/astronomy.c
    src/panchangacalculator.cpp
    src/bikram.cpp
    src/bsmonthdata.cpp
    src/eventsdata.cpp
    src/constants.cpp
)
target_link_libraries(run_test PRIVATE Qt6::Core)

add_test(NAME PanchangaTest COMMAND run_test)

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

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/.local" CACHE PATH "..." FORCE)
endif()

message(STATUS "Install prefix set to: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Binary will be installed to: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}")

install(TARGETS bikram-calendar
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#=============================================================================
# IDE Support
#=============================================================================
# Copy compile_commands.json to the source directory for Clangd/IDEs
add_custom_target(
    copy_compile_commands ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_SOURCE_DIR}/compile_commands.json
)
add_executable(test_bs tests/test_bs.cpp src/bikram.cpp src/bsmonthdata.cpp src/astronomy.c src/panchangacalculator.cpp src/eventsdata.cpp src/constants.cpp)
target_link_libraries(test_bs PRIVATE Qt6::Core)
