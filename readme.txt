For Debian / Ubuntu
First, install the essential build tools and Qt 6 development packages:

sudo apt update
sudo apt install build-essential \
                 qt6-base-dev \
                 qt6-declarative-dev \
                 qml6-module-qtquick \
                 qml6-module-qtquick-controls \
                 qml6-module-qtquick-layouts \
                 qml6-module-qtquick-controls-material \
                 qml6-module-qt-labs-settings

For Arch Linux
Install the base-devel group (if you haven't already) and the required Qt 6 packages:

sudo pacman -Syu
sudo pacman -S base-devel qt6-base qt6-declarative

2. Compilation Steps
Once the dependencies are installed, follow these steps to compile the project:

Navigate to the project directory:

qmake6
make

(Note: On some systems, the command might be qmake instead of qmake6)

use cmake in build folder like `cmake ..`   then `make`

Build the application:
Run make to compile the source code and link the application.

make

This will create an executable file in your project directory (or a build-specific subdirectory).

