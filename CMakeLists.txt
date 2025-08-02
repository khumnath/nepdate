cmake_minimum_required(VERSION 3.16)

project(bikram-calendar VERSION 1.0 LANGUAGES CXX)

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
    REQUIRED)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_add_executable(bikram-calendar
    main.cpp
    autostartmanager.cpp
    tooltipmanager.cpp
    resources.qrc
)

target_sources(bikram-calendar PRIVATE
    autostartmanager.h
    tooltipmanager.h
)

target_link_libraries(bikram-calendar PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Widgets
    Qt6::QuickControls2
)
