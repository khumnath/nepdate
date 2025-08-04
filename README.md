[Read this document in Nepali   [![नेपाली](https://img.shields.io/badge/Language-NE-green)](README.ne.md)


---
# Bikram Calendar

**Executable:** `bikram-calendar`

A desktop widget and calendar application for Bikram Sambat, built with the modern Qt 6 framework. It uses a native C++ backend for efficient system tasks.

## Application in action

<img width="1676" height="1764" alt="Image" src="https://github.com/user-attachments/assets/ce2bec13-de65-48a1-b69c-de2e60f88368" />

---

## Key Features

* **Desktop Widget & Calendar:** Provides a convenient, always-on desktop widget. Clicking the widget opens a full-featured calendar window.

* **Date Calculation & Panchanga Details:**
    * The calendar uses a hybrid system for date calculation:
        * **Pre-computed Data:** For the 90-year period from **2000 Baisakh to 2089 Chaitra**, the calendar uses pre-computed data for high accuracy. A small notification appears in the app when you are viewing dates outside of this primary range.
        * **Algorithmic Calculation:** For dates outside this range, it dynamically computes them based on traditional Surya Siddanta calculation logic.
    * **Daily Panchanga Information:**
        * The main calendar view displays the *Tithi* for each day.
        * Clicking on a specific day reveals comprehensive details including the **Tithi, Nakshatra, Yoga, and Karana**. It also shows debug information like the sun and moon's positions.

    > **Disclaimer on Accuracy:**
    > As the Surya Siddanta is an observational calendar, calculated timings can differ by minutes to hours compared to other sources. While every effort has been made to ensure accuracy, the data may contain mistakes. For official or ceremonial purposes, please always consult a calendar approved by the **Nepal Panchanga Nirnayak Samiti (नेपाल पञ्चाङ्ग निर्णायक समिति)**.

* **Customization & Themes:**
    * The calendar window includes an easy-to-use switch for toggling between **Light and Dark themes**. Your preference is saved and restored on the next launch.
    * Right-click the widget to open a context menu where you can configure font sizes and colors.

* **State Persistence:**
    * The application remembers and restores the position of the widget and the calendar window.
    * The size of the calendar window is also saved and restored automatically.
    * *(See Wayland Limitations below for exceptions)*.

* **System Integration:**
    * Uses an internal Qt C++ backend for handling system tasks like printing, managing autostart, and displaying helpful tooltips.

## Compatibility

* **Framework:** Built entirely on the Qt 6 framework.
* **Required Version:** Requires **Qt 6.0 or higher**. It has been well-tested on version **Qt 6.8.0** and **Qt 6.9**
* **Legacy Support:** Please note that Qt 5 is **not supported**.

## Wayland Support & Limitations

Due to the security design and protocols of the Wayland display server, some features behave differently:

* **Window Position:** The application cannot query window coordinates on Wayland, so it **cannot save or restore the position** of the widget or calendar window. However, the window **size** is saved and restored correctly.

* **Widget Click Action:** On Wayland, a click on the movable widget is interpreted by the compositor as a "drag/move" action, which prevents the click from being registered by the application.
    * **Workaround:** To open the calendar on Wayland, **right-click the widget** and select the **"Open Calendar"** button from the context menu.
 


## Building From Source

This guide provides instructions for compiling the project on popular Linux distributions.

---

## 1. Install Dependencies

You must first install the necessary build tools and libraries for your specific operating system.

### For Debian / Ubuntu

First, update your package list, then install the essential build tools and Qt 6 development packages.
#### Install build-time dependencies

```shell
    sudo apt update
    sudo apt install build-essential \
    qt6-base-dev \
    qt6-dev-tools \
    qt6-declarative-dev \
    qt6-declarative-dev-tools \
    qt6-tools-dev \
    qt6-tools-dev-tools \
    qt6-shadertools-dev \
    libqt6printsupport6 \
    qml6-module-qtquick-controls \
    qml6-module-qtquick-controls2 \
    qml6-module-qtquick-layouts \
    qml6-module-qtquick-window \
    qml6-module-qtcore
   
   ```
#### Install runtime dependencies
```shell
    sudo apt update
    sudo apt install libqt6core6 \
    libqt6gui6 \
    libqt6qml6 \
    libqt6quick6 \
    libqt6printsupport6 \
    qml6-module-qtquick-controls \
    qml6-module-qtquick-controls2 \
    qml6-module-qtquick-layouts \
    qml6-module-qtquick-window
   
   ```

### For Arch Linux

Ensure your system is up-to-date. Install the `base-devel` group (if you haven't already) and the required Qt 6 packages.


#### Synchronize package databases and update the system
```shell
sudo pacman -Syu

# Install build-time dependencies
sudo pacman -S base-devel qt6-base qt6-declarative qt6-shadertools qt6-tools

```

#### Install runtime dependencies(for pre-built binaries only)
```shell
sudo pacman -S qt6-base qt6-declarative
```

**Note:** If you encounter build errors, double-check that all the listed dependencies have been installed successfully.
* runtime dependencies should install with build dependencies. if downloaded prebuilt binary install runtime libraries only.


----------

## 2. Compilation Instructions

Once the dependencies are installed, you can compile the project using one of the following methods.

### Method A: Using Qt Creator (Recommended)

This is the most straightforward method for building the project.

1.  Open the `.pro` file in Qt Creator.
    
2.  Configure the project kit if prompted.
    
3.  Click the **Build** button (or press `Ctrl+B`) and then the **Run** button (or press `Ctrl+R`).
    

### Method B: Using qmake (Command Line)

1.  Navigate to the project's root directory.
    
2.  Create and enter a build directory:
    
    ```shell
    mkdir build && cd build
    
    ```
    
3.  Generate the Makefile using `qmake6`.
    
    
    ```shell
    # Note: On some older systems, you might need to use 'qmake' instead of 'qmake6'.
    qmake6 ..
    
    ```
    
4.  Compile the source code:
    
    
    ```shell
    make
    
    ```
    
5.  The compiled executable will be located in the `build` directory.
    

### Method C: Using CMake (Command Line)

1.  Navigate to the project's root directory.
    
2.  Create and enter a build directory:
    
    
    ```shell
    mkdir build && cd build
    
    ```
    
3.  Generate the build files with CMake:
   
    
    ```shell
    cmake ..
    
    ```
    
4.  Compile the source code:
   
    
    ```shell
    make
    
    ```
    
5.  The compiled executable will be located in the `build` directory.

---


## Contributing

Contributions are welcome! If you have a suggestion or want to report a bug, please feel free to open an issue.

If you would like to contribute code, please follow these steps:

1.  **Fork** the repository.
    
2.  Create a new branch (`git checkout -b feature/your-feature-name`).
    
3.  Make your changes and **commit** them (`git commit -m 'Add some feature'`).
    
4.  **Push** to the branch (`git push origin feature/your-feature-name`).
    
5.  Open a **Pull Request**.
    


## License

This project is licensed under the GNU General Public License v3.0 or later. See the [LICENSE](https://www.gnu.org/licenses/gpl-3.0.html "GPLV3") file for more details.
