cmake_minimum_required(VERSION 3.16)

project(PancangaCalculator VERSION 1.0 LANGUAGES CXX)

# Enable Qt features
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required Qt6 packages
find_package(Qt6 COMPONENTS 
    Core 
    Gui 
    Qml 
    Quick 
    QuickControls2 
    REQUIRED)

# Set Qt policies to suppress warnings
qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

# Add executable target
qt_add_executable(PancangaCalculator
    # Source files
    main.cpp
    autostartmanager.cpp
    # Resource files
    resources.qrc
)

# Add header files (required for MOC/headers)
target_sources(PancangaCalculator PRIVATE
    autostartmanager.h
)

# Set QML_SOURCES_DIR define as in .pro file
target_compile_definitions(PancangaCalculator PRIVATE QML_SOURCES_DIR="$$PWD/qml")

# Link Qt6 libraries
target_link_libraries(PancangaCalculator PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
)