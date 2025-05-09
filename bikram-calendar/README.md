# bikram-calendar

A comprehensive JavaScript/TypeScript library for working with the Bikram Sambat (BS) calendar system used in Nepal.  
Provides utilities for date conversion between Bikram Sambat and Gregorian calendars, tithi (lunar day) calculation, Nepali/English digit conversion, and more.

---

## Features

- Convert between Bikram Sambat (BS) and Gregorian (AD) dates
- Get information about Bikram months (days, start day of week)
- Calculate Tithi (Lunar day)
- Convert between Nepali and English digits
- Access month and day names in both Nepali and English

---

## Usage

> **Note:**  
> This package is **not published to npm**.  
> If you want to use it, **download or clone this repository** and copy or import the `bikram-calendar` folder into your own project.  
> You can then import from it using a relative path, for example:

```typescript
import {
  getToday,
  convertToBikram,
  convertToEnglish,
  calculateTithi,
  getNepaliDigits,
  getEnglishDigits,
  nepaliMonthsNp,
  nepaliMonthsEn
} from './bikram-calendar'; // Adjust the path as needed
```

---

## API Documentation

### Date Conversion

#### `getToday(): BikramDateObj`
Returns today's date in Bikram Sambat.

#### `convertToBikram(date: Date): BikramDateObj`
Converts a JavaScript Date to Bikram Sambat.

#### `convertToEnglish(bsDate: BikramDateObj): Date`
Converts a Bikram Sambat date to JavaScript Date.

### Tithi Calculation

#### `calculateTithi(date: Date): TithiResult`
Calculates the tithi (lunar day) for a given date.

### Month Information

#### `getBikramMonth(year: number, month: number): BikramMonth`
Gets information about a Bikram month including days and start day of week.

### Utilities

#### `getNepaliDigits(num: number): string`
Converts English digits to Nepali digits.

#### `getEnglishDigits(nepaliStr: string): string`
Converts Nepali digits to English digits.

#### `nepaliMonthsNp`, `nepaliMonthsEn`
Arrays of Nepali and English month names.

#### `nepaliDaysNp`, `nepaliDaysEn`
Arrays of Nepali and English day names.

---

## License
GNU General Public License v3.0 or later

This software is licensed under the GNU General Public License, which allows for free use, modification, and distribution. The GPL ensures that the software and any derivative works remain open and free. For full details, please visit [GNU GPL v3.0](https://www.gnu.org/licenses/gpl-3.0.en.html).


---

**Made with love by Khumnath.**