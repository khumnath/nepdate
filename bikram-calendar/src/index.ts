
// Export all functionality from a single entry point
export { BikramDate } from './bikram';
export { BS_START_YEAR, BS_END_YEAR } from './data';

export {
  getToday,
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
} from './converter';

export {
  calculateTithi,
  getTithiNamesNp,
  getTithiNamesEn,
  getPakshaNames,
  type TithiResult
} from './tithi';
