# Nepdate calendar
`nepdate-calendar` provides a standalone Nepali calendar and converter for Bikram Sambat and Gregorian calendars.

 bikram class converted to javascript and made date converter here https://khumnath.com.np/nepdate .
using this javascript calendar can be made. code is in page branch here https://github.com/khumnath/nepdate/tree/page .

# Nepdate widget

`nepdate-widget` is a desktop floating widget for Linux that displays the Nepali date. It provides the following features:

- Displays the Nepali date on your desktop.
- Can be moved anywhere on the screen.
- with automatic text color adjustment based on the background color.

##To-Do List

1. Create an installable DEB file for Debian-based distributions. (released appimages for wider linux distributions but gcc version is high now in release build. I will build with lower gcc version in future release.)
2. Make the calendar responsive to window resizing.(Done)
3. Implement tithi and other panchanga calculations in the calendar app.( Tithi implemented. in future i will check other components like nakshatra karan and yoga too.)
4. make cross plateform application. now nepdate-calendar and widget builds and runs perfectly in windows.
using qtcreator and mingw compiler. not tested cmake.(Done)

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
nepdate (v2.0.0) in action ![Screenshot_select-area_20240930185731](https://github.com/user-attachments/assets/93a88b55-4a24-47ce-beee-1c8334ccba3c)


## License

This project is licensed under the **GNU General Public License (GPL) version 3**. See the LICENSE file for details.

## contribution

first of all any suggestion and bug reports can be filed on github issue here https://github.com/khumnath/nepdate/issues

# for contribution 
1. Clone repository
 2. make changes
  3. make a pull request with proper description of changes.


