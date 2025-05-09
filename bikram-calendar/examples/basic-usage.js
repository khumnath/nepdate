
// Import the library (this would normally be 'require("bikram-calendar")' for users)
const { 
  getToday, 
  convertToBikram, 
  convertToEnglish,
  calculateTithi,
  getNepaliDigits,
  nepaliMonthsNp,
  nepaliMonthsEn 
} = require('../dist');

// Get today's date in Bikram Sambat
const today = getToday();
console.log(`Today in Bikram Sambat: ${today.year}-${today.month}-${today.day}`);
console.log(`Today in Nepali: ${getNepaliDigits(today.day)} ${nepaliMonthsNp[today.month - 1]} ${getNepaliDigits(today.year)}`);
console.log(`Today in English: ${today.day} ${nepaliMonthsEn[today.month - 1]} ${today.year}`);

// Convert a specific Gregorian date to Bikram
const gregorianDate = new Date(2023, 3, 14); // April 14, 2023
const bikramDate = convertToBikram(gregorianDate);
console.log(`\nConverting April 14, 2023 to Bikram Sambat:`);
console.log(`${bikramDate.year}-${bikramDate.month}-${bikramDate.day} BS`);

// Convert a specific Bikram date to Gregorian
const customBikramDate = {
  year: 2080,
  month: 1,
  day: 1,
  englishDate: new Date()
};
const englishDate = convertToEnglish(customBikramDate);
console.log(`\nConverting 2080-01-01 BS to Gregorian:`);
console.log(englishDate.toDateString());

// Calculate tithi (lunar day)
const tithiInfo = calculateTithi(new Date());
console.log(`\nToday's tithi:`);
console.log(`${tithiInfo.tithiNameEn} (${tithiInfo.tithiName})`);
console.log(`${tithiInfo.pakshaEn} (${tithiInfo.paksha})`);
