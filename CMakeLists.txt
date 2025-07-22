cmake_minimum_required(VERSION 3.16)

project(PancangaCalculator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

cmake_minimum_required(VERSION 3.16)

project(PancangaCalculator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the required Qt6 packages first.
find_package(Qt6 COMPONENTS Core Gui Qml Quick QuickControls2 REQUIRED)

# Now, set the Qt policies to remove the warnings.
qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

# Define the executable target and include the .qrc file as a source.
# CMAKE_AUTORCC will automatically handle the compilation of the .qrc file.
qt_add_executable(PancangaCalculator
    main.cpp
    resources.qrc
)

# The qt_add_resources command is no longer needed as the .qrc is now a source file.

# Finally, link the executable to the Qt libraries.
target_link_libraries(PancangaCalculator PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
)
