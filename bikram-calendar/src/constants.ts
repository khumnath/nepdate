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

/** Array of Tithi names (lunar days) in the Hindu calendar.
 * Contains 30 entries representing the lunar phases:
 * - First 15 entries are for Shukla Paksha (waxing moon phase)
 * - Last 15 entries are for Krishna Paksha (waning moon phase)
 * @readonly
 */
export const tithi = [
    "Pratipada", "Dwitiya", "Tritiya", "Chaturthi", "Panchami",
    "Shashthi", "Saptami", "Ashtami", "Navami", "Dashami", "Ekadashi",
    "Dwadashi", "Trayodashi", "Chaturdashi", "Purnima", "Pratipada",
    "Dwitiya", "Tritiya", "Chaturthi", "Panchami", "Shashthi",
    "Saptami", "Ashtami", "Navami", "Dashami", "Ekadashi",
    "Dwadashi", "Trayodashi", "Chaturdashi", "Amavasya"
];

/** Array of Nakshatra names (lunar mansions/constellations) in the Hindu calendar.
 * Contains 27 entries representing the celestial divisions along the ecliptic
 * through which the moon passes during its monthly cycle.
 * @readonly
 */
export const nakshatra = [
    "Ashwini", "Bharani", "Krittika", "Rohini", "Mrigashirsha", "Ardra",
    "Punarvasu", "Pushya", "Ashlesha", "Magha", "Purva Phalguni", "Uttara Phalguni",
    "Hasta", "Chitra", "Swati", "Vishakha", "Anuradha", "Jyeshtha", "Mula",
    "Purva Ashadha", "Uttara Ashadha", "Shravana", "Dhanishta", "Shatabhisha",
    "Purva Bhadrapada", "Uttara Bhadrapada", "Revati"
];

/** Array of Rashi names (zodiac signs) in the Hindu calendar.
 * Contains 12 entries representing the zodiacal constellations
 * through which the sun appears to pass during its yearly cycle.
 * @readonly
 */
export const rashi = [
    "Mesh", "Vrish", "Mithun", "Kark", "Singh", "Kanya",
    "Tula", "Vrischik", "Dhanu", "Makar", "Kumbh", "Meen"
];

/** Array of Karana names (half of a lunar day) in the Hindu calendar.
 * Contains 11 entries representing the different karanas that repeat
 * in a cycle throughout the lunar month.
 * @readonly
 */
export const karan = [
    "Bava", "Balava", "Kaulava", "Taitula", "Garija", "Vanija",
    "Visti", "Sakuni", "Chatuspada", "Naga", "Kimstughna"
];

/** Array of Yoga names in the Hindu calendar.
 * Contains 27 entries representing the angular relationship
 * between the sun and moon, calculated by their joint motion.
 * @readonly
 */
export const yoga = [
    "Vishkambha", "Prithi", "Ayushman", "Saubhagya", "Shobhana",
    "Atiganda", "Sukarman", "Dhrithi", "Shoola", "Ganda", "Vridhi",
    "Dhruva", "Vyaghata", "Harshana", "Vajra", "Siddhi", "Vyatipata",
    "Variyan", "Parigha", "Shiva", "Siddha", "Sadhya", "Shubha", "Shukla",
    "Bramha", "Indra", "Vaidhruthi"
];

/** Array of Solar Month names in the Hindu calendar (Nepal variant).
 * Contains 12 entries representing the solar months
 * as used in the traditional Nepali calendar.
 * @readonly
 */
export const solarMonthNames = [
    "Baisakh", "Jestha", "Ashadh", "Shrawan", "Bhadra", "Ashwin",
    "Kartik", "Mangsir", "Poush", "Magh", "Falgun", "Chaitra"
];

/** Array of Tithi names (lunar days) in the Hindu calendar (Nepali transliteration).
 * Contains 30 entries representing the lunar phases:
 * - First 15 entries are for Shukla Paksha (waxing moon phase)
 * - Last 15 entries are for Krishna Paksha (waning moon phase)
 * @readonly
 */
export const tithiNp = [
    "प्रतिपदा", "द्वितीया", "तृतीया", "चतुर्थी", "पञ्चमी",
    "षष्ठी", "सप्तमी", "अष्टमी", "नवमी", "दशमी", "एकादशी",
    "द्वादशी", "त्रयोदशी", "चतुर्दशी", "पूर्णिमा", "प्रतिपदा",
    "द्वितीया", "तृतीया", "चतुर्थी", "पञ्चमी", "षष्ठी",
    "सप्तमी", "अष्टमी", "नवमी", "दशमी", "एकादशी",
    "द्वादशी", "त्रयोदशी", "चतुर्दशी", "औंसी"
];

/** Array of Nakshatra names (lunar mansions/constellations) in the Hindu calendar (Nepali transliteration).
 * Contains 27 entries representing the celestial divisions along the ecliptic
 * through which the moon passes during its monthly cycle.
 * @readonly
 */
export const nakshatraNp = [
    "अश्विनी", "भरणी", "कृत्तिका", "रोहिणी", "मृगशिरा", "आर्द्रा",
    "पुनर्वसु", "पुष्य", "आश्लेषा", "मघा", "पूर्व फाल्गुनी", "उत्तर फाल्गुनी",
    "हस्त", "चित्रा", "स्वाति", "विशाखा", "अनुराधा", "ज्येष्ठा", "मूल",
    "पूर्वाषाढा", "उत्तराषाढा", "श्रवण", "धनिष्ठा", "शतभिषा",
    "पूर्व भाद्रपद", "उत्तर भाद्रपद", "रेवती"
];

/** Array of Rashi names (zodiac signs) in the Hindu calendar (Nepali transliteration).
 * Contains 12 entries representing the zodiacal constellations
 * through which the sun appears to pass during its yearly cycle.
 * @readonly
 */
export const rashiNp = [
    "मेष", "वृष", "मिथुन", "कर्कट", "सिंह", "कन्या",
    "तुला", "वृश्चिक", "धनु", "मकर", "कुम्भ", "मीन"
];

/** Array of Karana names (half of a lunar day) in the Hindu calendar (Nepali transliteration).
 * Contains 11 entries representing the different karanas that repeat
 * in a cycle throughout the lunar month.
 * @readonly
 */
export const karanNp = [
    "बव", "बालव", "कौलव", "तैतिल", "गर", "वणिज",
    "विष्टि", "शकुनि", "चतुष्पद", "नाग", "किंस्तुघ्न"
];

/** Array of Yoga names in the Hindu calendar (Nepali transliteration).
 * Contains 27 entries representing the angular relationship
 * between the sun and moon, calculated by their joint motion.
 * @readonly
 */
export const yogaNp = [
    "विश्कम्भ", "प्रीति", "आयुष्मान", "सौभाग्य", "शोभन",
    "अतिगण्ड", "सुकर्मा", "धृति", "शूल", "गण्ड", "वृद्धि",
    "ध्रुव", "व्याघात", "हर्षण", "वज्र", "सिद्धि", "व्यतीपात",
    "वरीयान", "परिघ", "शिव", "सिद्ध", "साध्य", "शुभ", "शुक्ल",
    "ब्रह्मा", "इन्द्र", "वैधृति"
];

/** Array of Solar Month names in the Hindu calendar (Nepal variant) (Nepali transliteration).
 * Contains 12 entries representing the solar months
 * as used in the traditional Nepali calendar.
 * @readonly
 */
export const solarMonthNamesNp = [
    "बैशाख", "जेठ", "असार", "श्रावण", "भदौ", "आश्विन",
    "कार्तिक", "मंसिर", "पुष", "माघ", "फाल्गुन", "चैत्र"
];
