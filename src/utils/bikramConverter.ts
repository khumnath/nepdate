// Re-export all needed functionality from the bikram-calendar package
import {
  getToday as originalGetToday,
  getBikramMonth,
  convertToBikram,
  convertToEnglish,
  getNepaliDigits,
  getEnglishDigits,
  containsNepaliDigits,
  nepaliMonthsNp,
  nepaliMonthsEn,
  nepaliDaysNp,
  nepaliDaysEn,
  type BikramDateObj,
  type BikramMonth
} from '../../bikram-calendar/src/converter';

export function getToday(date?: Date): BikramDateObj {
  const today = date || new Date();
  const bikramDate = convertToBikram(today);
  return {
    year: bikramDate.year,
    month: bikramDate.month,
    day: bikramDate.day,
    englishDate: today
  };
}

export function getNepalTodayDate(): Date {
  const now = new Date();
  const utc = now.getTime() + (now.getTimezoneOffset() * 60000);
  const nepalOffsetMs = 5.75 * 60 * 60 * 1000;
  const nepalNow = new Date(utc + nepalOffsetMs);
  return new Date(
    nepalNow.getFullYear(),
    nepalNow.getMonth(),
    nepalNow.getDate(),
    0, 0, 0, 0
  );
}

export {
  getBikramMonth,
  convertToBikram,
  convertToEnglish,
  getNepaliDigits,
  getEnglishDigits,
  containsNepaliDigits,
  nepaliMonthsNp,
  nepaliMonthsEn,
  nepaliDaysNp,
  nepaliDaysEn,
  type BikramDateObj,
  type BikramMonth
};
