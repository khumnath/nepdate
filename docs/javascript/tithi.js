function calculateTithi(year, month, day) {
    // Nepal Time Offset (UTC+5:45) in minutes
    const nepalTimeOffsetMinutes = 5 * 60 + 45; // 5 hours and 45 minutes in minutes
    const localOffsetMinutes = new Date().getTimezoneOffset(); // This is negative for UTC+ and positive for UTC-
    const totalOffsetMinutes = nepalTimeOffsetMinutes - localOffsetMinutes;
    const adjustedDay = day - Math.floor(totalOffsetMinutes / (60 * 24)); // Adjust for day rollover

    const tdays = calculateDaysSinceJ2000(year, month, adjustedDay);
    const t = tdays / 36525.0;
    const tithiIndex = calculateTithiIndex(t);
    return {
        tithi: getTithiName(tithiIndex),
        paksha: getPaksha(tithiIndex)
    };
}

function calculateDaysSinceJ2000(year, month, day) {
    const julianDate = getJulianDate(year, month, day);
    return julianDate - 2451545.0; // J2000.0 Julian date
}

function getJulianDate(year, month, day) {
    if (month <= 2) {
        year -= 1;
        month += 12;
    }
    const a = Math.floor(year / 100);
    const b = 2 - a + Math.floor(a / 4);
    return Math.floor(365.25 * (year + 4716)) + Math.floor(30.6001 * (month + 1)) + day + b - 1524.5;
}

function calculateTithiIndex(t) {
    const moonLongitude = getMoonLongitude(t);
    const sunLongitude = getSunLongitude(t);

    let difference = moonLongitude - sunLongitude;
    if (difference < 0) difference += 360.0;

    return Math.floor(difference / 12.0);
}

function getSunLongitude(t) {
    const l0 = 280.4665 + 36000.7698 * t;
    const m = 357.5291 + 35999.0503 * t;
    const c = (1.9146 - 0.004817 * t - 0.000014 * t * t) * Math.sin(m * Math.PI / 180)
              + (0.019993 - 0.000101 * t) * Math.sin(2 * m * Math.PI / 180)
              + 0.00029 * Math.sin(3 * m * Math.PI / 180);
    const theta = l0 + c;
    const lambda = theta - 0.00569 - 0.00478 * Math.sin((125.04 - 1934.136 * t) * Math.PI / 180);
    return lambda % 360;
}

function getMoonLongitude(t) {
    let L1 = 218.316 + 481267.8813 * t;
    let M1 = 134.963 + 477198.8676 * t;
    
    L1 = L1 % 360;
    M1 = M1 % 360;

    const longitude = (L1 + 6.289 * Math.sin(M1 * Math.PI / 180)) % 360;
    return longitude;
}

function getTithiName(tithiIndex) {
    const tithiNames = [
        "प्रथमा", "द्वितीया", "तृतीया", "चतुर्थी", "पंचमी", "षष्ठी", "सप्तमी", "अष्टमी",
        "नवमी", "दशमी", "एकादशी", "द्वादशी", "त्रयोदशी", "चतुर्दशी", "पूर्णिमा", "प्रथमा", 
        "द्वितीया", "तृतीया", "चतुर्थी", "पंचमी", "षष्ठी", "सप्तमी", "अष्टमी", "नवमी", "दशमी", 
        "एकादशी", "द्वादशी", "त्रयोदशी", "चतुर्दशी", "अमावस्या"
    ];
    return tithiNames[tithiIndex];
}

function getPaksha(tithiIndex) {
    return (tithiIndex < 15) ? "शुक्ल पक्ष" : "कृष्ण पक्ष"; // Shukla Paksha or Krishna Paksha
}

// Example usage:
const today = new Date();
const year = today.getFullYear();
const month = today.getMonth() + 1; // Months are zero-based in JavaScript
const day = today.getDate();

const result = calculateTithi(year, month, day);

console.log(`Tithi: ${result.tithi}, Paksha: ${result.paksha}, Today: ${year}-${month}-${day}`);
