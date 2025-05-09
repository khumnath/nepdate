
/**
 * Tithi calculation utilities
 * 
 * This module calculates the lunar day (tithi) for a given Gregorian date.
 * 
 * Copyright (c) 2024 Khumnath CG and onwards
 * Licensed under the GNU General Public License v3.0 or later.
 */

// Tithi (lunar day) names in Nepali
const tithiNamesNp = [
  'प्रतिपदा', 'द्वितीया', 'तृतीया', 'चतुर्थी', 'पञ्चमी',
  'षष्ठी', 'सप्तमी', 'अष्टमी', 'नवमी', 'दशमी',
  'एकादशी', 'द्वादशी', 'त्रयोदशी', 'चतुर्दशी', 'पूर्णिमा',
  'प्रतिपदा', 'द्वितीया', 'तृतीया', 'चतुर्थी', 'पञ्चमी',
  'षष्ठी', 'सप्तमी', 'अष्टमी', 'नवमी', 'दशमी',
  'एकादशी', 'द्वादशी', 'त्रयोदशी', 'चतुर्दशी', 'औंसी'
];

// Tithi (lunar day) names in English
const tithiNamesEn = [
  'Pratipada', 'Dwitiya', 'Tritiya', 'Chaturthi', 'Panchami',
  'Sasthi', 'Saptami', 'Ashtami', 'Navami', 'Dashami',
  'Ekadashi', 'Dwadashi', 'Trayodashi', 'Chaturdashi', 'Purnima',
  'Pratipada', 'Dwitiya', 'Tritiya', 'Chaturthi', 'Panchami',
  'Sasthi', 'Saptami', 'Ashtami', 'Navami', 'Dashami',
  'Ekadashi', 'Dwadashi', 'Trayodashi', 'Chaturdashi', 'Amavasya'
];

// Paksha (lunar fortnight) names in Nepali and English
const pakshaNames = {
  np: ['शुक्ल पक्ष', 'कृष्ण पक्ष'],
  en: ['Shukla Paksha', 'Krishna Paksha']
};

export interface TithiResult {
  tithiIndex: number;    // 0-29
  tithiName: string;     // Name in Nepali
  tithiNameEn: string;   // Name in English
  paksha: string;        // Paksha in Nepali
  pakshaEn: string;      // Paksha in English
  pakshaIndex: number;   // 0 for Shukla, 1 for Krishna
}

/**
 * Calculates the Tithi (lunar day) for a given date
 * 
 * @param date - JavaScript Date object
 * @returns TithiResult object with tithi details
 */
export function calculateTithi(date: Date): TithiResult {
  // Constants for tithi calculation
  const TITHI_DAYS = 30;
  const MOON_CYCLE_DAYS = 29.5305882; // Average lunar month in days
  
  // Base date for calculation (known new moon day)
  const baseDate = new Date(2023, 0, 21); // January 21, 2023 - known Amavasya
  
  // Calculate days elapsed since base date
  const msPerDay = 24 * 60 * 60 * 1000;
  const daysElapsed = Math.abs((date.getTime() - baseDate.getTime()) / msPerDay);
  
  // Calculate tithi index (0-29)
  const tithiIndex = Math.floor((daysElapsed % MOON_CYCLE_DAYS) / (MOON_CYCLE_DAYS / TITHI_DAYS));
  
  // Determine paksha (lunar fortnight)
  const pakshaIndex = tithiIndex < 15 ? 0 : 1; // 0 for Shukla, 1 for Krishna
  
  return {
    tithiIndex,
    tithiName: tithiNamesNp[tithiIndex],
    tithiNameEn: tithiNamesEn[tithiIndex],
    paksha: pakshaNames.np[pakshaIndex],
    pakshaEn: pakshaNames.en[pakshaIndex],
    pakshaIndex
  };
}

/**
 * Get all tithi names in Nepali
 */
export function getTithiNamesNp(): string[] {
  return tithiNamesNp;
}

/**
 * Get all tithi names in English
 */
export function getTithiNamesEn(): string[] {
  return tithiNamesEn;
}

/**
 * Get paksha names
 */
export function getPakshaNames(): { np: string[], en: string[] } {
  return pakshaNames;
}
