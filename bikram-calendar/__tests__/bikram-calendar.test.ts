
import { 
  getToday, 
  convertToBikram, 
  convertToEnglish, 
  getNepaliDigits, 
  getEnglishDigits, 
  calculateTithi,
  BikramDate
} from '../src';

describe('Bikram Calendar', () => {
  test('getToday returns current date in Bikram Sambat', () => {
    const today = getToday();
    expect(today).toHaveProperty('year');
    expect(today).toHaveProperty('month');
    expect(today).toHaveProperty('day');
    expect(today).toHaveProperty('englishDate');
  });

  // Tests may not be accurate due to limited  time available to cross-check with actual Bikram dates
  test('convertToBikram converts Gregorian to Bikram Sambat date', () => {
    // April 14, 2023 = 1 Baisakh 2080
    const gregorianDate = new Date(2023, 3, 14);
    const bikramDate = convertToBikram(gregorianDate);
    
    expect(bikramDate.year).toBe(2080);
    expect(bikramDate.month).toBe(1);
    expect(bikramDate.day).toBe(1);
  });

  test('convertToEnglish converts Bikram Sambat to Gregorian date', () => {
    // 1 Baisakh 2080 = April 14, 2023
    const bikramDate = {
      year: 2080,
      month: 1,
      day: 1,
      englishDate: new Date() // This will be overwritten
    };
    
    const gregorianDate = convertToEnglish(bikramDate);
    
    expect(gregorianDate.getFullYear()).toBe(2023);
    expect(gregorianDate.getMonth()).toBe(3); // April (0-indexed)
    expect(gregorianDate.getDate()).toBe(14);
  });

  test('getNepaliDigits converts English digits to Nepali', () => {
    const nepaliDigits = getNepaliDigits(2080);
    expect(nepaliDigits).toBe('२०८०');
  });

  test('getEnglishDigits converts Nepali digits to English', () => {
    const englishDigits = getEnglishDigits('२०८०');
    expect(englishDigits).toBe('2080');
  });

  test('calculateTithi returns tithi information for a date', () => {
    // Use a specific date for fixed test
    const date = new Date(2023, 0, 21); // Known Amavasya (new moon)
    const tithi = calculateTithi(date);
    
    expect(tithi).toHaveProperty('tithiName');
    expect(tithi).toHaveProperty('tithiNameEn');
    expect(tithi).toHaveProperty('paksha');
    expect(tithi).toHaveProperty('pakshaEn');
    
    // Verify this is an Amavasya (new moon)
    expect(tithi.tithiNameEn).toBe('Amavasya');
  });

  test('BikramDate class can be instantiated', () => {
    const date = new BikramDate(2080, 1, 1);
    expect(date.getYear()).toBe(2080);
    expect(date.getMonth()).toBe(1);
    expect(date.getDate()).toBe(1);
  });
});
