# Nepdate calendar
`nepdate-calendar` provides a standalone Nepali calendar and converter for Bikram Sambat and Gregorian calendars.

# Nepdate widget

`nepdate-widget` is a desktop floating widget for Linux that displays the Nepali date. It provides the following features:

- Displays the Nepali date on your desktop.
- Can be moved anywhere on the screen.
- with automatic text color adjustment based on the background color.

##To-Do List

1. Create an installable DEB file for Debian-based distributions.
2. Make the calendar responsive to window resizing.
3. Implement tithi and other panchanga calculations in the calendar app.
4. Create a cross-platform application. now nepdate-calendar builds and runs perfectly 
using qtcreator and mingw compiler. not tested cmake. nepdate-widget is building but 
does  not display widget window.

## Dependencies

Nepdate and calendar relies on the following Qt libraries:

- Qt Core
- Qt Widgets
- Qt GUI

## Usage

1. Clone this repository to your local machine.
2. Create a build folder in the project directory.
3. Build the project using `cmake ..` and then run `make all`.
4. You can also compile the project using Qt Creator or Qmake.
5. Run the Nepdate executables:
    `nepdate-widget` for the desktop widget
    `nepdate-calendar` for the calendar
6. The floating Nepali date widget will appear on your desktop, with automatic text color adjustment based on the background color.

## Screenshots
nepdate in action ![nepdate](https://github.com/khumnath/nepdate/assets/50103558/c7f8c83a-29a7-44d6-985b-aa35ddf69954)


## License

This project is licensed under the **GNU General Public License (GPL) version 3**. See the LICENSE file for details.

## contribution

first of all any suggestion and bug reports can be filed on github issue here https://github.com/khumnath/nepdate/issues

# for contribution 
1. Clone repository
 2. make changes
  3. make a pull request with proper description of changes.


