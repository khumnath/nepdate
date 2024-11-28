function calculateTithi(year, month, day) {
    // Nepal Time Offset (UTC+5:45) in minutes
    const nepalTimeOffsetMinutes = 5 * 60 + 45; // 5 hours and 45 minutes in minutes

    // Get the current local time zone offset in minutes
    const localOffsetMinutes = new Date().getTimezoneOffset(); // This is negative for UTC+ and positive for UTC-

    // Calculate the total offset to Nepal time
    const totalOffsetMinutes = nepalTimeOffsetMinutes - localOffsetMinutes;

    // Adjust the day based on the total offset
    const adjustedDay = day - Math.floor(totalOffsetMinutes / (60 * 24)); // Adjust for day rollover

    const tdays = calculateDaysSinceJ2000(year, month, adjustedDay);
    const tithiIndex = calculateTithiIndex(tdays);
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

function calculateTithiIndex(tdays) {
    const moonLongitude = getMoonLongitude(tdays);
    const sunLongitude = getSunLongitude(tdays);

    let difference = moonLongitude - sunLongitude;
    if (difference < 0) difference += 360.0;

    return Math.floor(difference / 12.0);
}

function getMoonLongitude(tdays) {
    return (218.316 + 13.176396 * tdays) % 360;
}

function getSunLongitude(tdays) {
    return (280.46 + 0.9856474 * tdays) % 360;
}

function getTithiName(tithiIndex) {
    const tithiNames = [
        "प्रतिपदा", "द्वितीया", "तृतीया", "चतुर्थी", "पंचमी", "षष्ठी", "सप्तमी", "अष्टमी",
        "नवमी", "दशमी", "एकादशी", "द्वादशी", "त्रयोदशी", "चतुर्दशी", "पूर्णिमा", "प्रतिपदा", 
        "द्वितीया", "तृतीया", "चतुर्थी", "पंचमी", "षष्ठी", "सप्तमी", "अष्टमी", "नवमी", "दशमी", 
        "एकादशी", "द्वादशी", "त्रयोदशी", "चतुर्दशी", "अमावास्या"
    ];
    return tithiNames[(tithiIndex + 1) % 30]; // Adjusting for array index
}

function getPaksha(tithiIndex) {
    return (tithiIndex < 15) ? "शुक्ल पक्ष" : "कृष्ण पक्ष"; // Shukla Paksha or Krishna Paksha
}