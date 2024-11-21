function calculateTithi(year, month, day) {
    const tdays = calculateDaysSinceJ2000(year, month, day);
    const tithiIndex = calculateTithiIndex(tdays);
    return {
        tithi: getTithiName(tithiIndex + 1),
        paksha: getPaksha(tithiIndex)
    };
}

function calculateDaysSinceJ2000(year, month, day) {
    const julianDate = getJulianDate(year, month, day);
    return julianDate - 2451545.0;
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
        "प्रथमा", "द्वितीया", "तृतीया", "चतुर्थी", "पंचमी", "षष्ठी", "सप्तमी", "अष्टमी",
        "नवमी", "दशमी", "एकादशी", "द्वादशी", "त्रयोदशी", "चतुर्दशी", "पूर्णिमा", "अमावास्या"
    ];
    return tithiNames[tithiIndex % 15];
}

function getPaksha(tithiIndex) {
    return (tithiIndex < 15) ? "शुक्ल पक्ष" : "कृष्ण पक्ष";
}
