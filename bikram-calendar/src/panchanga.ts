/**
 * Panchanga Calculator - Hindu Astrological Calendar Calculator
 * Copyright (C) 2025 Khumnath Cg <nath.khum@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

import { tithi, nakshatra, rashi, karan, yoga, solarMonthNames } from './constants';


// --- Constants ---
const D2R = Math.PI / 180;
const R2D = 180 / Math.PI;

const REV = (x: number): number => x - Math.floor(x / 360) * 360;

// Global variables (mean anomaly and mean longitude)
let Ls_global = 0;
let Ms_global = 0;
let Lm_global = 0;
let Mm_global = 0;

// --- Julian Day calculation ---
function julianDay(yy: number, mm: number, dd: number, hour: number, zhr: number): number {
    const hourUTC = hour - zhr;
    if (mm <= 2) {
        yy -= 1;
        mm += 12;
    }
    const A = Math.floor(yy / 100);
    const B = 2 - A + Math.floor(A / 4);
    const jd = Math.floor(365.25 * (yy + 4716)) + Math.floor(30.6001 * (mm + 1)) + dd + B - 1524.5;
    return jd + hourUTC / 24;
}

// --- Pad single digit numbers with leading zero ---
function pad(n: number): string {
    return n.toString().padStart(2, '0');
}

// --- Calculate sunrise or sunset time for a given date ---
function calculateSunriseOrSunset(date: Date, isSunrise: boolean): string {
    const latitude = 27.7172;
    const longitude = 85.324;
    const nepalOffset = 5.75;
    const year = date.getFullYear();
    const month = date.getMonth() + 1;
    const day = date.getDate();
    const N1 = Math.floor(275 * month / 9);
    const N2 = Math.floor((month + 9) / 12);
    const N3 = (1 + Math.floor((year - 4 * Math.floor(year / 4) + 2) / 3));
    const N = N1 - (N2 * N3) + day - 30;
    const zenith = 90.8333;
    const lngHour = longitude / 15;
    const t_approx_factor = isSunrise ? 6 : 18;
    const t_approx = N + ((t_approx_factor - lngHour) / 24);
    const M_sun = (0.9856 * t_approx) - 3.289;
    let L_sun = M_sun + (1.916 * Math.sin(M_sun * D2R)) + (0.020 * Math.sin(2 * M_sun * D2R)) + 282.634;
    L_sun = REV(L_sun);
    let RA_sun = R2D * Math.atan(0.91764 * Math.tan(L_sun * D2R));
    RA_sun = REV(RA_sun);
    const L_quadrant = Math.floor(L_sun / 90) * 90;
    const RA_quadrant = Math.floor(RA_sun / 90) * 90;
    RA_sun = RA_sun + (L_quadrant - RA_quadrant);
    RA_sun = RA_sun / 15;
    const sinDec_sun = 0.39782 * Math.sin(L_sun * D2R);
    const cosDec_sun = Math.cos(Math.asin(sinDec_sun));
    const cosH = (Math.cos(zenith * D2R) - (sinDec_sun * Math.sin(latitude * D2R))) / (cosDec_sun * Math.cos(latitude * D2R));
    if (cosH > 1) return "Sun never rises";
    if (cosH < -1) return "Sun never sets";
    let H_angle_hours;
    if (isSunrise) {
        H_angle_hours = (360 - R2D * Math.acos(cosH)) / 15;
    } else {
        H_angle_hours = (R2D * Math.acos(cosH)) / 15;
    }
    const T_ut = H_angle_hours + RA_sun - (0.06571 * t_approx) - 6.622;
    let UT_event = T_ut - lngHour;
    UT_event = REV(UT_event * 360 / 24) / 15;
    let localT_event = UT_event + nepalOffset;
    localT_event = REV(localT_event * 360 / 24) / 15;
    const hours_local = Math.floor(localT_event);
    const minutes_local = Math.floor((localT_event - hours_local) * 60);
    return `${pad(hours_local)}:${pad(minutes_local)}`;
}

// --- Format time in AM/PM ---
function formatAMPM(hours: number, minutes: number): string {
    const h_12 = hours % 12 || 12;
    const ampm = hours < 12 || hours === 24 ? 'AM' : 'PM';
    return `${h_12}:${pad(Math.floor(minutes))} ${ampm}`;
}

// --- Convert Julian Day to date parts ---
function jdToDateParts(jd: number, timezoneOffsetHours: number) {
    const jd_unix_epoch_offset = 2440587.5;
    const ms_per_day = 86400000;
    const utcMillis = (jd - jd_unix_epoch_offset) * ms_per_day;
    const targetMillis = utcMillis + timezoneOffsetHours * 60 * 60 * 1000;
    const dateInTargetTimezone = new Date(targetMillis);
    return {
        year: dateInTargetTimezone.getFullYear(),
        month: dateInTargetTimezone.getMonth() + 1,
        day: dateInTargetTimezone.getDate(),
        hours: dateInTargetTimezone.getHours(),
        minutes: dateInTargetTimezone.getMinutes(),
        seconds: dateInTargetTimezone.getSeconds()
    };
}

// --- Calculate Tithi times for a given date ---
function calculateTithiTimes(dateForPanchanga: Date) {
    const localYear = dateForPanchanga.getFullYear();
    const localMonth = dateForPanchanga.getMonth() + 1;
    const localDay = dateForPanchanga.getDate();
    const zhr_nepal = 5.75;
    const sunriseStr = calculateSunriseOrSunset(dateForPanchanga, true);
    if (sunriseStr.includes("never")) {
        return { tithiStart: "N/A", tithiEnd: "N/A", error: sunriseStr };
    }
    const [sunriseHourStr, sunriseMinStr] = sunriseStr.split(':');
    const sunriseHour = parseInt(sunriseHourStr, 10);
    const sunriseMin = parseInt(sunriseMinStr, 10);
    const jdSunrise = julianDay(localYear, localMonth, localDay, sunriseHour + sunriseMin / 60, zhr_nepal);
    function tithiAngleAt(jd_calc: number) {
        const d_calc = jd_calc - 2451545.0;
        const ayanamsa_val = calc_ayanamsa(d_calc);
        const slon_trop = sun_long(d_calc);
        const mlon_trop = moon_long(d_calc);
        const siderealMoon_calc = REV(mlon_trop + ayanamsa_val); 
        const siderealSun_calc = REV(slon_trop + ayanamsa_val);
        const elongation = siderealMoon_calc - siderealSun_calc;
        return REV(elongation);
    }
    

function findTithiBoundary(
    searchStartJD: number,
    searchEndJD: number,
    targetBoundaryAngle: number,
    tithiAngleFunc: (jd: number) => number,
    contextLog: string = ""
): number {
    let lowJD = searchStartJD;
    let highJD = searchEndJD;
    let midJD;
    let iterations = 0;
    const maxIterations = 100; // Max iterations to prevent infinite loops
    const normalizedTargetAngle = REV(targetBoundaryAngle);
    const angleAtLow = REV(tithiAngleFunc(lowJD));
    const angleAtHigh = REV(tithiAngleFunc(highJD));

    if (angleAtLow <= angleAtHigh) {
        if (!(angleAtLow <= normalizedTargetAngle && normalizedTargetAngle <= angleAtHigh)) {
            return -1; // Optional: Handle unbracketed case if needed
        }
    } else {
        if (!(angleAtLow <= normalizedTargetAngle || normalizedTargetAngle <= angleAtHigh)) {
            return -1; // Optional: Handle unbracketed case if needed
        }
    }

    // Precision target: interval smaller than 1 second
    while ((highJD - lowJD) * 86400.0 > 1 && iterations < maxIterations) {
        midJD = (lowJD + highJD) / 2;
        if (midJD === lowJD || midJD === highJD) { // Prevent infinite loop if JDs become identical due to precision
            break;
        }
        const angleAtMid = REV(tithiAngleFunc(midJD));
        const diff = REV(angleAtMid - normalizedTargetAngle + 180) - 180;

        if (diff < 0) {
            lowJD = midJD;
        } else {
            highJD = midJD;
        }
        iterations++;
    }
    // --- Test ---
    if (iterations >= maxIterations) {
        console.warn(
            `IMPROVED SEARCH (${contextLog}): Max iterations (${maxIterations}) reached for target ${normalizedTargetAngle.toFixed(4)}.\n` +
            `  Final Window JD: [${lowJD.toFixed(6)}, ${highJD.toFixed(6)}]. Result might be imprecise.`
        );
    }
    return (lowJD + highJD) / 2;
}

    const tithiAngleAtSunrise = tithiAngleAt(jdSunrise);
    const currentTithiIndex = Math.floor(tithiAngleAtSunrise / 12);
    const currentTithiStartBoundaryAngle = currentTithiIndex * 12;
    const nextTithiStartBoundaryAngle = (currentTithiIndex + 1) * 12;
    const tithiStartJD = findTithiBoundary(jdSunrise - 1.5, jdSunrise + 0.5, currentTithiStartBoundaryAngle, tithiAngleAt);
    const tithiEndJD = findTithiBoundary(jdSunrise - 0.5, jdSunrise + 1.5, nextTithiStartBoundaryAngle, tithiAngleAt);
    const tithiStartParts = jdToDateParts(tithiStartJD, zhr_nepal);
    const tithiEndParts = jdToDateParts(tithiEndJD, zhr_nepal);
    const tithiStartTimeStr = `${pad(tithiStartParts.day)}/${pad(tithiStartParts.month)}/${tithiStartParts.year} ${formatAMPM(tithiStartParts.hours, tithiStartParts.minutes)}`;
    const tithiEndTimeStr = `${pad(tithiEndParts.day)}/${pad(tithiEndParts.month)}/${tithiEndParts.year} ${formatAMPM(tithiEndParts.hours, tithiEndParts.minutes)}`;
    return { tithiStart: tithiStartTimeStr, tithiEnd: tithiEndTimeStr };
}

// --- Tithi and Paksha Names ---
const tithiNamesNp = [
  'प्रतिपदा', 'द्वितीया', 'तृतीया', 'चतुर्थी', 'पञ्चमी',
  'षष्ठी', 'सप्तमी', 'अष्टमी', 'नवमी', 'दशमी',
  'एकादशी', 'द्वादशी', 'त्रयोदशी', 'चतुर्दशी', 'पूर्णिमा',
  'प्रतिपदा', 'द्वितीया', 'तृतीया', 'चतुर्थी', 'पञ्चमी',
  'षष्ठी', 'सप्तमी', 'अष्टमी', 'नवमी', 'दशमी',
  'एकादशी', 'द्वादशी', 'त्रयोदशी', 'चतुर्दशी', 'औंसी'
];
const tithiNamesEn = [
  'Pratipada', 'Dwitiya', 'Tritiya', 'Chaturthi', 'Panchami',
  'Sasthi', 'Saptami', 'Ashtami', 'Navami', 'Dashami',
  'Ekadashi', 'Dwadashi', 'Trayodashi', 'Chaturdashi', 'Purnima',
  'Pratipada', 'Dwitiya', 'Tritiya', 'Chaturthi', 'Panchami',
  'Sasthi', 'Saptami', 'Ashtami', 'Navami', 'Dashami',
  'Ekadashi', 'Dwadashi', 'Trayodashi', 'Chaturdashi', 'Amavasya'
];
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
 * Calculates the Tithi (lunar day) for a given date using accurate Panchanga calculations
 * @param date - JavaScript Date object
 * @returns TithiResult object with tithi details
 */
export function calculateTithi(date: Date): TithiResult {
  // Use accurate Panchanga calculations for tithi
  const localYear = date.getFullYear();
  const localMonth = date.getMonth() + 1;
  const localDay = date.getDate();
  const zhr_nepal = 5.75;
  const sunriseStr = calculateSunriseOrSunset(date, true);
  if (sunriseStr.includes("never")) {
    return {
      tithiIndex: -1,
      tithiName: '',
      tithiNameEn: '',
      paksha: '',
      pakshaEn: '',
      pakshaIndex: -1
    };
  }
  const [sHourStr, sMinStr] = sunriseStr.split(':');
  const sunriseHourDecimal = parseInt(sHourStr, 10) + parseInt(sMinStr, 10) / 60;
  const jdAtSunrise = julianDay(localYear, localMonth, localDay, sunriseHourDecimal, zhr_nepal);
  const d_days_since_j2000 = jdAtSunrise - 2451543.5;
  const ayanamsa = calc_ayanamsa(d_days_since_j2000);
  const slon_tropical = sun_long(d_days_since_j2000);
  const mlon_tropical = moon_long(d_days_since_j2000);
  const siderealMoon = REV(mlon_tropical + ayanamsa);
  const siderealSun = REV(slon_tropical + ayanamsa);
  const tithiAngle = (siderealMoon - siderealSun < 0) ? siderealMoon - siderealSun + 360 : siderealMoon - siderealSun;
  const tithiIndex = Math.floor(tithiAngle / 12);
  const pakshaIndex = tithiIndex < 15 ? 0 : 1;
  return {
    tithiIndex,
    tithiName: tithiNamesNp[tithiIndex],
    tithiNameEn: tithiNamesEn[tithiIndex],
    paksha: pakshaNames.np[pakshaIndex],
    pakshaEn: pakshaNames.en[pakshaIndex],
    pakshaIndex
  };
}

export function getTithiNamesNp(): string[] {
  return tithiNamesNp;
}

export function getTithiNamesEn(): string[] {
  return tithiNamesEn;
}

export function getPakshaNames(): { np: string[], en: string[] } {
  return pakshaNames;
}

// --- Calculate Sun's longitude ---
function sun_long(d: number): number {
    const w = 282.9404 + 4.70935e-5 * d;
    const e = 0.016709 - 1.151e-9 * d;
    const M = REV(356.0470 + 0.9856002585 * d);

    Ms_global = M;
    Ls_global = REV(w + M);

    const E = M + R2D * e * Math.sin(M * D2R) * (1 + e * Math.cos(M * D2R));

    const xv = Math.cos(E * D2R) - e;
    const yv = Math.sqrt(1 - e * e) * Math.sin(E * D2R);
    const v = REV(R2D * Math.atan2(yv, xv));

    return REV(v + w);
}

// --- Calculate Moon's longitude ---
function moon_long(d: number): number {
    const N = REV(125.1228 - 0.0529538083 * d);
    const i = 5.1454;
    const w = REV(318.0634 + 0.1643573223 * d);
    const a = 60.2666;
    const e = 0.054900;
    const M = REV(115.3654 + 13.0649929509 * d);

    // Set Moon Mean Anomaly and Mean Longitude for debug output
    const L0 = REV(218.316 + 13.176396 * d);
    Mm_global = M;  // Set Moon Mean Anomaly
    Lm_global = L0; // Set Moon Mean Longitude

    let E = M + R2D * e * Math.sin(M * D2R) * (1 + e * Math.cos(M * D2R));
    let Et;
    let count = 0;
    do {
        Et = E;
        const tmpE_D2R = Et * D2R;
        E = Et - (Et - R2D * e * Math.sin(tmpE_D2R) - M) / (1 - e * Math.cos(tmpE_D2R));
        count++;
    } while (Math.abs(E - Et) > 0.005 && count < 10);

    const xv = a * (Math.cos(E * D2R) - e);
    const yv = a * Math.sqrt(1 - e * e) * Math.sin(E * D2R);
    const v_true_anomaly = REV(R2D * Math.atan2(yv, xv));
    const r_dist = Math.sqrt(xv * xv + yv * yv);

    const xh = r_dist * (Math.cos(N * D2R) * Math.cos((v_true_anomaly + w) * D2R) - Math.sin(N * D2R) * Math.sin((v_true_anomaly + w) * D2R) * Math.cos(i * D2R));
    const yh = r_dist * (Math.sin(N * D2R) * Math.cos((v_true_anomaly + w) * D2R) + Math.cos(N * D2R) * Math.sin((v_true_anomaly + w) * D2R) * Math.cos(i * D2R));

    let moonLon = REV(R2D * Math.atan2(yh, xh));

    // Apply periodic corrections
    const D_elong = REV(L0 - sun_long(d));
    const F_arg_lat = REV(L0 - N);

    moonLon += -1.274 * Math.sin((M - 2 * D_elong) * D2R);
    moonLon += +0.658 * Math.sin((2 * D_elong) * D2R);
    moonLon += -0.186 * Math.sin(Ms_global * D2R);
    moonLon += -0.059 * Math.sin((2 * M - 2 * D_elong) * D2R);
    moonLon += -0.057 * Math.sin((M - 2 * D_elong + Ms_global) * D2R);
    moonLon += +0.053 * Math.sin((M + 2 * D_elong) * D2R);
    moonLon += +0.046 * Math.sin((2 * D_elong - Ms_global) * D2R);
    moonLon += +0.041 * Math.sin((M - Ms_global) * D2R);
    moonLon += -0.035 * Math.sin(D_elong * D2R);
    moonLon += -0.031 * Math.sin((M + Ms_global) * D2R);
    moonLon += -0.015 * Math.sin((2 * F_arg_lat - 2 * D_elong) * D2R);
    moonLon += +0.011 * Math.sin((M - 4 * D_elong) * D2R);

    return REV(moonLon);
}

function crossedRashiBoundary(prev: number, curr: number, boundary: number): boolean {
    prev = (prev + 360) % 360;
    curr = (curr + 360) % 360;
    boundary = (boundary + 360) % 360;
    if (boundary === 0 && prev > curr) return true;
    return (prev < boundary && curr >= boundary) || (prev > curr && curr >= boundary);
}

function calculateSolarDayDetails(jd: number, siderealSun: number) {
    const currentRashi = Math.floor(siderealSun / 30) * 30;
    const nextRashi = (currentRashi + 30) % 360;

    // Find start of current solar month (max 33 days back)
    let monthStartJD = jd;
    let iter = 0;
    let prevSun = siderealSun;
    let foundStart = false;
    while (iter < 33) {
        monthStartJD -= 1;
        const d = monthStartJD - 2451545.0;
        const slon = sun_long(d);
        const ayan = calc_ayanamsa(d);
        const currSun = REV(slon + ayan); // Changed back to + to match reference
        if (crossedRashiBoundary(currSun, prevSun, currentRashi)) {
            monthStartJD += 1;
            foundStart = true;
            break;
        }
        prevSun = currSun;
        iter++;
    }
    if (!foundStart) monthStartJD = jd - 32;

    // Find end of current solar month (max 33 days forward)
    let monthEndJD = jd;
    iter = 0;
    prevSun = siderealSun;
    let foundEnd = false;
    while (iter < 33) {
        monthEndJD += 1;
        const d = monthEndJD - 2451545.0;
        const slon = sun_long(d);
        const ayan = calc_ayanamsa(d);
        const currSun = REV(slon + ayan); // Changed back to + to match reference
        if (crossedRashiBoundary(prevSun, currSun, nextRashi)) {
            foundEnd = true;
            break;
        }
        prevSun = currSun;
        iter++;
    }
    if (!foundEnd) monthEndJD = jd + 32;

    let totalDays = Math.round(monthEndJD - monthStartJD);
    if (totalDays < 27 || totalDays > 32) totalDays = 32; // Changed back to 32 to match reference

    const currentDay = Math.floor((siderealSun % 30) + 0.5) + 1; // Removed 1.1 multiplier to match reference

    return {
        day: currentDay,
        totalDays: totalDays,
        progress: (siderealSun % 30) / 30
    };
}

/**
 * Calculates Ayanamsa (precession of the equinoxes).
 */
function calc_ayanamsa(d: number): number {
    // Using Lahiri Ayanamsa calculation
    const t = d / 36525; // Julian centuries since J2000
    
    // Precession in longitude
    const p = (5028.796195 - 1.1054348 * t) * t / 3600;
    
    // Lahiri correction
    const lahiriCorrection = 23.856045 + 0.0001631 * t;
    
    // Final ayanamsa value
    return -1 * (p + lahiriCorrection);
}

// --- Main calculation ---
export function calculatePanchanga(dateObj: Date) {
    const localYear = dateObj.getFullYear();
    const localMonth = dateObj.getMonth() + 1;
    const localDay = dateObj.getDate();
    const zhr_nepal = 5.75;

    const sunriseStr = calculateSunriseOrSunset(dateObj, true);
    const sunsetStr = calculateSunriseOrSunset(dateObj, false);

    if (sunriseStr.includes("never")) {
        return { error: "Sunrise calculation error: " + sunriseStr };
    }
    const [sHourStr, sMinStr] = sunriseStr.split(':');
    const sunriseHourDecimal = parseInt(sHourStr, 10) + parseInt(sMinStr, 10) / 60;

    const jdAtSunrise = julianDay(localYear, localMonth, localDay, sunriseHourDecimal, zhr_nepal);
    const d_days_since_j2000 = jdAtSunrise - 2451543.5; // Changed to match reference

    const ayanamsa = calc_ayanamsa(d_days_since_j2000);

    const slon_tropical = sun_long(d_days_since_j2000);
    const mlon_tropical = moon_long(d_days_since_j2000);

    const siderealMoon = REV(mlon_tropical + ayanamsa); // Changed back to + to match reference
    const siderealSun = REV(slon_tropical + ayanamsa);  // Changed back to + to match reference

    const tithiAngle = (siderealMoon - siderealSun < 0) ? siderealMoon - siderealSun + 360 : siderealMoon - siderealSun;
    const tithiIndex = Math.floor(tithiAngle / 12);
    const dtithi = tithi[tithiIndex];
    const dpaksha = tithiIndex < 15 ? "Shukla" : "Krishna";

    const nakshatraIndex = Math.floor(siderealMoon / (360 / 27));
    const dnakshatra = nakshatra[nakshatraIndex];

    const yogaAngle = REV(siderealSun + siderealMoon);
    const yogaIndex = Math.floor(yogaAngle / (360 / 27));
    const dyoga = yoga[yogaIndex];

    const rashiIndex = Math.floor(siderealMoon / 30);
    const drashi = rashi[rashiIndex];

    const karanaSubIndex = Math.floor(tithiAngle / 6);
    let dkarana;
    if (karanaSubIndex === 0) dkarana = karan[10];
    else if (karanaSubIndex === 57) dkarana = karan[7];
    else if (karanaSubIndex === 58) dkarana = karan[8];
    else if (karanaSubIndex === 59) dkarana = karan[9];
    else {
        dkarana = karan[(karanaSubIndex - 1) % 7];
    }

    const solarMonthIndex = Math.floor(siderealSun / 30);
    const solarDetails = calculateSolarDayDetails(jdAtSunrise, siderealSun);
    const dsolarMonth = rashi[solarMonthIndex];
    const dsolarDay = solarDetails.day;
    const daysInMonth = solarDetails.totalDays;
    const solarMonthName = solarMonthNames[solarMonthIndex];

    const { tithiStart, tithiEnd, error: tithiTimeError } = calculateTithiTimes(dateObj);
    if (tithiTimeError) {
        return { error: "Tithi time calculation error: " + tithiTimeError };
    }

    // Match debug output format exactly with reference
    const debugOutput = `\n    Debug Information:\n        Date (Input Local): ${dateObj.toLocaleDateString()} (${localYear}-${pad(localMonth)}-${pad(localDay)})\n        Sunrise (Nepal): ${sunriseStr} | Sunset (Nepal): ${sunsetStr}\n        JD at Sunrise: ${jdAtSunrise.toFixed(6)}\n        Days since J2000: ${d_days_since_j2000.toFixed(6)}\n        Ayanamsa: ${ayanamsa.toFixed(4)}°\n        Sun Tropical Lon: ${slon_tropical.toFixed(4)}° (Mean Anom: ${Ms_global.toFixed(4)}°, Mean Lon: ${Ls_global.toFixed(4)}°)\n        Moon Tropical Lon: ${mlon_tropical.toFixed(4)}° (Mean Anom: ${Mm_global.toFixed(4)}°, Mean Lon: ${Lm_global.toFixed(4)}°)\n        Sun Sidereal Lon: ${siderealSun.toFixed(4)}°\n        Moon Sidereal Lon: ${siderealMoon.toFixed(4)}°\n        Tithi Angle : ${tithiAngle.toFixed(4)}° => Index: ${tithiIndex} => ${tithi[tithiIndex]} (${dpaksha})\n        Tithi Timing: ${tithiStart} | Tithi End: ${tithiEnd}\n        Nakshatra   : Index=> ${nakshatraIndex} => ${nakshatra[nakshatraIndex]}\n        Yoga Angle  : ${yogaAngle.toFixed(4)}° => Index: ${yogaIndex} => ${dyoga}\n        Rashi (Moon): Index => ${rashiIndex} => ${rashi[rashiIndex]}\n        Karana      : Index => ${karanaSubIndex} => ${dkarana}\n        Solar Month : Index => ${solarMonthIndex} => ${solarMonthNames[solarMonthIndex]}\n        Solar Day   : ${dsolarDay} / ${daysInMonth} (Progress: ${(solarDetails.progress * 100).toFixed(2)}%)\n    `;

    return {
        dtithi: dtithi || 'N/A',
        dpaksha: dpaksha || 'N/A',
        dnakshatra: dnakshatra || 'N/A',
        dyoga: dyoga || 'N/A',
        dkarana: dkarana || 'N/A',
        drashi: drashi || 'N/A',
        dsolarMonth: dsolarMonth || 'N/A',
        dsolarDay: dsolarDay || 'N/A',
        daysInMonth: daysInMonth || 'N/A',
        solarMonthName: solarMonthName || 'N/A',
        tithiStart: tithiStart || 'N/A',
        tithiEnd: tithiEnd || 'N/A',
        sunrise: sunriseStr || 'N/A',
        sunset: sunsetStr || 'N/A',
        debug: debugOutput
    };
}

// Add the PanchangaResult type for TypeScript consumers
export type PanchangaResult = {
    dtithi: string;
    dpaksha: string;
    dnakshatra: string;
    dyoga: string;
    dkarana: string;
    drashi: string;
    dsolarMonth: string;
    dsolarDay: string | number;
    daysInMonth: string | number;
    solarMonthName: string;
    tithiStart: string;
    tithiEnd: string;
    sunrise: string;
    sunset: string;
    debug: string;
    error?: string;
};

// Export all calculation functions and constants for use in App.tsx
export {
  tithi,
  nakshatra,
  rashi,
  karan,
  yoga,
  solarMonthNames,
  REV,
  D2R,
  R2D,
  calc_ayanamsa,
  sun_long,
  moon_long,
  julianDay,
  pad,
  calculateSunriseOrSunset,
  formatAMPM,
  jdToDateParts,
  calculateTithiTimes,
  crossedRashiBoundary,
  calculateSolarDayDetails
};