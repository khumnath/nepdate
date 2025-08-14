// EventsData.js
// This file contains event data for the Panchanga Calculator.

.pragma library

// Lunar based events, defined by month, fortnight (paksha), and lunar day (tithi).
var lunarEvents = [
    {
        eventType: "lunar",
        lunarMonth: "चैत्र",
        paksha: "शुक्ल पक्ष",
        tithi: "अष्टमी",
        event: "चैते दशैँ",
        detail: "चैत्र महिनामा मनाइने दशैँ, ठूलो दशैँको सानो रूप।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "चैत्र",
        paksha: "शुक्ल पक्ष",
        tithi: "नवमी",
        event: "रामनवमी",
        detail: "भगवान रामको जन्मदिन।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "चैत्र",
        paksha: "कृष्ण पक्ष",
        tithi: "अमावस्या",
        event: "मातातीर्थ औंसी",
        detail: "आमाको मुख हेर्ने दिन।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "चैत्र",
        paksha: "कृष्ण पक्ष",
        tithi: "अमावस्या",
        event: "घोडेजात्रा",
        detail: "काठमाडौँमा मनाइने घोडे जात्रा उत्सव।",
        category: "Jatra"
    },
    {
        eventType: "lunar",
        lunarMonth: "वैशाख",
        paksha: "शुक्ल पक्ष",
        tithi: "तृतीया",
        event: "अक्षय तृतीया",
        detail: "जौ सातु खाने दिन।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "वैशाख",
        paksha: "शुक्ल पक्ष",
        tithi: "पूर्णिमा",
        event: "बुद्ध जयन्ती / उभौली पर्व",
        detail: "भगवान बुद्धको जन्मदिन र किरात समुदायको उभौली पर्व।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "आषाढ",
        paksha: "शुक्ल पक्ष",
        tithi: "पूर्णिमा",
        event: "गुरु पूर्णिमा",
        detail: "गुरु र शिक्षकहरूलाई सम्मान गर्ने दिन।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "श्रावण",
        paksha: "शुक्ल पक्ष",
        tithi: "पञ्चमी",
        event: "नाग पञ्चमी",
        detail: "सर्प देवताको पूजा गर्ने दिन।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "श्रावण",
        paksha: "शुक्ल पक्ष",
        tithi: "पूर्णिमा",
        event: "जनै पूर्णिमा / रक्षा बन्धन",
        detail: "जनै बदल्ने तथा रक्षा बन्धन बांध्ने  पवित्र उत्सव।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "श्रावण",
        paksha: "कृष्ण पक्ष",
        tithi: "अष्टमी",
        event: "कृष्ण जन्माष्टमी",
        detail: "भगवान कृष्णको जन्मदिन।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "भाद्रपद",
        paksha: "कृष्ण पक्ष",
        tithi: "प्रतिपदा",
        event: "गाईजात्रा",
        detail: "गाईको उत्सव, हास्य र व्यङ्ग्यको मेल।",
        category: "Jatra"
    },
    {
        eventType: "lunar",
        lunarMonth: "भाद्रपद",
        paksha: "कृष्ण पक्ष",
        tithi: "अमावस्या",
        event: "कुशे औंसी",
        detail: "बुवाको मुख हेर्ने दिन।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "भाद्रपद",
        paksha: "शुक्ल पक्ष",
        tithi: "तृतीया",
        event: "हरितालिका तीज",
        detail: "महिलाहरूले को महान उत्सव।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "भाद्रपद",
        paksha: "शुक्ल पक्ष",
        tithi: "चतुर्थी",
        event: "गणेश चतुर्थी",
        detail: "भगवान गणेशको जन्मदिन।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "भाद्रपद",
        paksha: "शुक्ल पक्ष",
        tithi: "चतुर्दशी",
        event: "इन्द्रजात्रा",
        detail: "काठमाडौँको प्रमुख सडक उत्सवको मुख्य दिन।",
        category: "Jatra"
    },
    {
        eventType: "lunar",
        lunarMonth: "आश्विन",
        paksha: "शुक्ल पक्ष",
        tithi: "प्रतिपदा",
        event: "घटस्थापना",
        detail: "दशैँको सुरुवात, नवरात्रिको पहिलो दिन।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "आश्विन",
        paksha: "शुक्ल पक्ष",
        tithi: "सप्तमी",
        event: "फूलपाती",
        detail: "दशैँको सातौँ दिन।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "आश्विन",
        paksha: "शुक्ल पक्ष",
        tithi: "अष्टमी",
        event: "महाअष्टमी",
        detail: "दशैँको आठौँ दिन।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "आश्विन",
        paksha: "शुक्ल पक्ष",
        tithi: "नवमी",
        event: "महानवमी",
        detail: "दशैँको नवौं दिन।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "आश्विन",
        paksha: "शुक्ल पक्ष",
        tithi: "दशमी",
        event: "विजयादशमी (दशैं)",
        detail: "दशैँको दशौँ दिन, विजयको दिन।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "आश्विन",
        paksha: "शुक्ल पक्ष",
        tithi: "पूर्णिमा",
        event: "कोजाग्रत पूर्णिमा",
        detail: "दशैँको अन्त्य।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "कार्तिक",
        paksha: "कृष्ण पक्ष",
        tithi: "त्रयोदशी",
        event: "काग तिहार",
        detail: "तिहारको पहिलो दिन, कागलाई समर्पित।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "कार्तिक",
        paksha: "कृष्ण पक्ष",
        tithi: "चतुर्दशी",
        event: "कुकुर तिहार",
        detail: "तिहारको दोस्रो दिन, कुकुरलाई समर्पित।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "कार्तिक",
        paksha: "कृष्ण पक्ष",
        tithi: "अमावस्या",
        event: "लक्ष्मी पूजा",
        detail: "तिहारको तेस्रो दिन, देवी लक्ष्मीको पूजा।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "कार्तिक",
        paksha: "शुक्ल पक्ष",
        tithi: "प्रतिपदा",
        event: "गोवर्धन पूजा / म्ह पूजा",
        detail: "तिहारको चौथो दिन,  आत्म-पूजाको दिन।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "कार्तिक",
        paksha: "शुक्ल पक्ष",
        tithi: "द्वितीया",
        event: "भाइटीका",
        detail: "तिहारको पाँचौँ दिन, दिदी-भाइबीचको प्रेमको दिन।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "कार्तिक",
        paksha: "शुक्ल पक्ष",
        tithi: "षष्ठी",
        event: "छठ पर्व",
        detail: "सूर्य देवतालाई समर्पित उत्सव, मुख्यतया तराईमा मनाइन्छ।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "कार्तिक",
        paksha: "शुक्ल पक्ष",
        tithi: "एकादशी",
        event: "हरिबोधिनी एकादशी",
        detail: "भगवान विष्णुको जागरणको दिन।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "मार्गशीर्ष",
        paksha: "शुक्ल पक्ष",
        tithi: "पञ्चमी",
        event: "विवाह पञ्चमी",
        detail: "भगवान राम र सीताको विवाहको सम्झना।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "मार्गशीर्ष",
        paksha: "शुक्ल पक्ष",
        tithi: "पूर्णिमा",
        event: "य:मरि पुन्हि / धान्यपुर्णिमा",
        detail: "नेवार समुदायको यमरि पुन्हि / धान्य पूर्णिमाको उत्सव।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "माघ",
        paksha: "शुक्ल पक्ष",
        tithi: "पञ्चमी",
        event: "सरस्वती पूजा / वसन्तपञ्चमी",
        detail: "ज्ञान र शिक्षाको देवी सरस्वतीको पूजा।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "फाल्गुन",
        paksha: "कृष्ण पक्ष",
        tithi: "चतुर्दशी",
        event: "महाशिवरात्रि",
        detail: "भगवान शिवको महान रात्रि।",
        category: "धार्मिक"
    },
    {
        eventType: "lunar",
        lunarMonth: "फाल्गुन",
        paksha: "शुक्ल पक्ष",
        tithi: "पूर्णिमा",
        event: "होली पूर्णिमा",
        detail: "रङ्गहरूको उत्सव।",
        category: "धार्मिक"
    }
];

// Gregorian events for every year.
var gregorianEvents = [
    {
        event: "New Year's Day",
        dateType: "gregorian",
        date: "01/01",
        detail: "Celebration of the new Gregorian year.",
        category: "International Day"
    },
    {
        event: "Valentine's Day",
        dateType: "gregorian",
        date: "02/14",
        detail: "International day of love. Nepali: प्रणय दिवस.",
        category: "International Day"
    },
    {
        event: "International Women's Day",
        dateType: "gregorian",
        date: "03/08",
        detail: "Global day celebrating womanhood. Nepali: नारी दिवस.",
        category: "International Day"
    },
    {
        event: "World Health Day",
        dateType: "gregorian",
        date: "04/07",
        detail: "A global health awareness day. Nepali: विश्व स्वास्थ्य दिवस.",
        category: "International Day"
    },
    {
        event: "International Workers' Day",
        dateType: "gregorian",
        date: "05/01",
        detail: "Also known as May Day. Nepali: अन्तर्राष्ट्रिय श्रमिक दिवस.",
        category: "International Day"
    },
    {
        event: "World Environment Day",
        dateType: "gregorian",
        date: "06/05",
        detail: "Day for environmental awareness and action. Nepali: विश्व वातावरण दिवस.",
        category: "International Day"
    },
    {
        event: "International Day of Yoga",
        dateType: "gregorian",
        date: "06/21",
        detail: "Global day to celebrate yoga. Nepali: विश्व योग दिवस.",
        category: "International Day"
    },
    {
        event: "World Population Day",
        dateType: "gregorian",
        date: "07/11",
        detail: "Day to raise awareness of global population issues. Nepali: विश्व जनसंख्या दिवस.",
        category: "International Day"
    },
    {
        event: "International Youth Day",
        dateType: "gregorian",
        date: "08/12",
        detail: "Day to celebrate young people as agents of change. Nepali: अन्तर्राष्ट्रिय युवा दिवस.",
        category: "International Day"
    },
    {
        event: "International Human Rights Day",
        dateType: "gregorian",
        date: "12/10",
        detail: "Commemorates the adoption of the Universal Declaration of Human Rights. Nepali: अन्तर्राष्ट्रिय मानव अधिकार दिवस.",
        category: "International Day"
    },
    {
        event: "Christmas Day",
        dateType: "gregorian",
        date: "12/25",
        detail: "Christian festival celebrating the birth of Jesus. Nepali: क्रिसमस-डे.",
        category: "Festival"
    }
];

// Bikram recurring events (based on BS month/day).
var bikramRecurringEvents = [
    {
        event: "लोकतन्त्र दिवस",
        dateType: "brecurring",
        date: "01/11",
        detail: "लोकतन्त्र दिवस, हरेक वर्ष बैशाख ११ गते मनाइन्छ।",
        category: "National Day"
    },
    {
        event: "गणतन्त्र दिवस",
        dateType: "brecurring",
        date: "02/15",
        detail: "गणतन्त्र दिवस, हरेक वर्ष जेठ १५ गते मनाइन्छ।",
        category: "National Day"
    },
    {
        event: "राष्ट्रिय धान दिवस",
        dateType: "brecurring",
        date: "03/15",
        detail: "राष्ट्रिय धान दिवस, हरेक वर्ष असार १५ गते मनाइन्छ।",
        category: "National Day"
    },
    {
        event: "भानु जयन्ती",
        dateType: "brecurring",
        date: "03/29",
        detail: "कवि भानुभक्त आचार्यको जन्म जयन्ती।",
        category: "Jayanti"
    },
    {
        event: "संविधान दिवस",
        dateType: "brecurring",
        date: "06/03",
        detail: "संविधान दिवस, हरेक वर्ष असोज ३ गते मनाइन्छ।",
        category: "National Day"
    },
    {
        event: "पृथ्वी जयन्ती / राष्ट्रिय एकता दिवस",
        dateType: "brecurring",
        date: "09/27",
        detail: "पृथ्वी जयन्ती / राष्ट्रिय एकता दिवस।",
        category: "National Day"
    },
    {
        event: "शहीद दिवस",
        dateType: "brecurring",
        date: "10/16",
        detail: "शहीद दिवस।",
        category: "National Day"
    },
    {
        event: "प्रजातन्त्र दिवस",
        dateType: "brecurring",
        date: "11/07",
        detail: "राणा शासनको अन्त्यको सम्झनामा मनाइने प्रजातन्त्र दिवस।",
        category: "National Day"
    },
    {
        event: "नयाँ वर्ष",
        dateType: "brecurring",
        date: "01/01",
        detail: "नेपाली नयाँ वर्ष।",
        category: "Festival"
    },
    {
        event: "बिस्का: जात्रा",
        dateType: "brecurring",
        date: "01/01",
        detail: "भक्तपुरमा मनाइने बिस्का जात्राको उत्सव।",
        category: "Jatra"
    },
    {
        event: "साउने सङ्क्रान्ति",
        dateType: "brecurring",
        date: "04/01",
        detail: "श्रावण सङ्क्रान्ति, श्रावण महिनाको पहिलो दिन।",
        category: "Sankranti"
    },
    {
        event: "माघे संक्रान्ति",
        dateType: "brecurring",
        date: "10/01",
        detail: "माघे सङ्क्रान्ति, माघ महिनाको पहिलो दिन।",
        category: "Sankranti"
    }
];

// Bikram fixed events (specific BS year/month/day).
var bikramFixedEvents = [
    {
        event: "भोटो जात्रा",
        dateType: "bikram",
        date: "2082/02/18",
        detail: "रातो मच्छिन्द्रनाथ जात्राको समापन दिन।",
        category: "Jatra"
    },
    {
        event: "तमु ल्होसार",
        dateType: "bikram",
        date: "2082/09/15",
        detail: "गुरुङ समुदायको नयाँ वर्ष।",
        category: "Lhosar"
    },
    {
        event: "सोनाम ल्होसार",
        dateType: "bikram",
        date: "2082/10/05",
        detail: "तमाङ समुदायको नयाँ वर्ष।",
        category: "Lhosar"
    },
    {
        event: "ग्याल्पो ल्होछार",
        dateType: "bikram",
        date: "2082/11/06",
        detail: "शेर्पा समुदायको नयाँ वर्ष।",
        category: "Lhosar"
    },
    {
        event: "महा शिवरात्री / नेपाली सेना दिवस",
        dateType: "bikram",
        date: "2082/11/04",
        detail: "भगवान शिवको महान रात्रि र नेपाली सेनाको दिवस।",
        category: "Festival"
    }
];
