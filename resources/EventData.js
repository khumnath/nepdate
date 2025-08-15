// EventsData.js
// This file contains event data for the Panchanga Calculator, organized by type.
// All events have a consistent structure including a 'holiday' boolean.

.pragma library

// Lunar based events, defined by month, fortnight (paksha), and lunar day (tithi).
var lunarEvents = [
    {
        event: "चैते दशैँ",
        eventType: "lunar",
        lunarMonth: "चैत्र",
        paksha: "शुक्ल पक्ष",
        tithi: "अष्टमी",
        startYear: null,
        endYear: null,
        detail: "चैत्र महिनामा मनाइने दशैँ, ठूलो दशैँको सानो रूप।",
        holiday: true
    },
    {
        event: "रामनवमी",
        eventType: "lunar",
        lunarMonth: "चैत्र",
        paksha: "शुक्ल पक्ष",
        tithi: "नवमी",
        startYear: null,
        endYear: null,
        detail: "भगवान रामको जन्मदिन।",
        holiday: true
    },
    {
        event: "मातातीर्थ औंसी",
        eventType: "lunar",
        lunarMonth: "चैत्र",
        paksha: "कृष्ण पक्ष",
        tithi: "अमावस्या",
        startYear: null,
        endYear: null,
        detail: "आमाको मुख हेर्ने दिन。",
        holiday: true
    },
    {
        event: "घोडेजात्रा",
        eventType: "lunar",
        lunarMonth: "चैत्र",
        paksha: "कृष्ण पक्ष",
        tithi: "अमावस्या",
        startYear: null,
        endYear: null,
        detail: "काठमाडौँमा मनाइने घोडे जात्रा उत्सव।",
        holiday: true
    },
    {
        event: "अक्षय तृतीया",
        eventType: "lunar",
        lunarMonth: "वैशाख",
        paksha: "शुक्ल पक्ष",
        tithi: "तृतीया",
        startYear: null,
        endYear: null,
        detail: "जौ सातु खाने दिन。",
        holiday: false
    },
    {
        event: "बुद्ध जयन्ती / उभौली पर्व",
        eventType: "lunar",
        lunarMonth: "वैशाख",
        paksha: "शुक्ल पक्ष",
        tithi: "पूर्णिमा",
        startYear: null,
        endYear: null,
        detail: "भगवान बुद्धको जन्मदिन र किरात समुदायको उभौली पर्व।",
        holiday: true
    },
    {
        event: "गुरु पूर्णिमा",
        eventType: "lunar",
        lunarMonth: "आषाढ",
        paksha: "शुक्ल पक्ष",
        tithi: "पूर्णिमा",
        startYear: null,
        endYear: null,
        detail: "गुरु र शिक्षकहरूलाई सम्मान गर्ने दिन।",
        holiday: false
    },
    {
        event: "नाग पञ्चमी",
        eventType: "lunar",
        lunarMonth: "श्रावण",
        paksha: "शुक्ल पक्ष",
        tithi: "पञ्चमी",
        startYear: null,
        endYear: null,
        detail: "सर्प देवताको पूजा गर्ने दिन।",
        holiday: false
    },
    {
        event: "जनै पूर्णिमा / रक्षा बन्धन",
        eventType: "lunar",
        lunarMonth: "श्रावण",
        paksha: "शुक्ल पक्ष",
        tithi: "पूर्णिमा",
        startYear: null,
        endYear: null,
        detail: "जनै बदल्ने तथा रक्षा बन्धन बांध्ने  पवित्र उत्सव।",
        holiday: true
    },
    {
        event: "कृष्ण जन्माष्टमी",
        eventType: "lunar",
        lunarMonth: "श्रावण",
        paksha: "कृष्ण पक्ष",
        tithi: "अष्टमी",
        startYear: null,
        endYear: null,
        detail: "भगवान कृष्णको जन्मदिन।",
        holiday: true
    },
    {
        event: "गाईजात्रा",
        eventType: "lunar",
        lunarMonth: "भाद्रपद",
        paksha: "कृष्ण पक्ष",
        tithi: "प्रतिपदा",
        startYear: null,
        endYear: null,
        detail: "गाईको उत्सव, हास्य र व्यङ्ग्यको मेल।",
        holiday: true
    },
    {
        event: "कुशे औंसी",
        eventType: "lunar",
        lunarMonth: "भाद्रपद",
        paksha: "कृष्ण पक्ष",
        tithi: "अमावस्या",
        startYear: null,
        endYear: null,
        detail: "बुवाको मुख हेर्ने दिन।",
        holiday: true
    },
    {
        event: "हरितालिका तीज",
        eventType: "lunar",
        lunarMonth: "भाद्रपद",
        paksha: "शुक्ल पक्ष",
        tithi: "तृतीया",
        startYear: null,
        endYear: null,
        detail: "महिलाहरूले को महान उत्सव।",
        holiday: true
    },
    {
        event: "गणेश चतुर्थी",
        eventType: "lunar",
        lunarMonth: "भाद्रपद",
        paksha: "शुक्ल पक्ष",
        tithi: "चतुर्थी",
        startYear: null,
        endYear: null,
        detail: "भगवान गणेशको जन्मदिन।",
        holiday: true
    },
    {
        event: "इन्द्रजात्रा",
        eventType: "lunar",
        lunarMonth: "भाद्रपद",
        paksha: "शुक्ल पक्ष",
        tithi: "चतुर्दशी",
        startYear: null,
        endYear: null,
        detail: "काठमाडौँको प्रमुख सडक उत्सवको मुख्य दिन।",
        holiday: true
    },
    {
        event: "घटस्थापना",
        eventType: "lunar",
        lunarMonth: "आश्विन",
        paksha: "शुक्ल पक्ष",
        tithi: "प्रतिपदा",
        startYear: null,
        endYear: null,
        detail: "दशैँको सुरुवात, नवरात्रिको पहिलो दिन।",
        holiday: true
    },
    {
        event: "फूलपाती",
        eventType: "lunar",
        lunarMonth: "आश्विन",
        paksha: "शुक्ल पक्ष",
        tithi: "सप्तमी",
        startYear: null,
        endYear: null,
        detail: "दशैँको सातौँ दिन।",
        holiday: true
    },
    {
        event: "महाअष्टमी",
        eventType: "lunar",
        lunarMonth: "आश्विन",
        paksha: "शुक्ल पक्ष",
        tithi: "अष्टमी",
        startYear: null,
        endYear: null,
        detail: "दशैँको आठौँ दिन।",
        holiday: true
    },
    {
        event: "महानवमी",
        eventType: "lunar",
        lunarMonth: "आश्विन",
        paksha: "शुक्ल पक्ष",
        tithi: "नवमी",
        startYear: null,
        endYear: null,
        detail: "दशैँको नवौं दिन।",
        holiday: true
    },
    {
        event: "विजयादशमी (दशैं)",
        eventType: "lunar",
        lunarMonth: "आश्विन",
        paksha: "शुक्ल पक्ष",
        tithi: "दशमी",
        startYear: null,
        endYear: null,
        detail: "दशैँको दशौँ दिन, विजयको दिन।",
        holiday: true
    },
    {
        event: "कोजाग्रत पूर्णिमा",
        eventType: "lunar",
        lunarMonth: "आश्विन",
        paksha: "शुक्ल पक्ष",
        tithi: "पूर्णिमा",
        startYear: null,
        endYear: null,
        detail: "दशैँको अन्त्य।",
        holiday: true
    },
    {
        event: "काग तिहार",
        eventType: "lunar",
        lunarMonth: "कार्तिक",
        paksha: "कृष्ण पक्ष",
        tithi: "त्रयोदशी",
        startYear: null,
        endYear: null,
        detail: "तिहारको पहिलो दिन, कागलाई समर्पित।",
        holiday: true
    },
    {
        event: "कुकुर तिहार",
        eventType: "lunar",
        lunarMonth: "कार्तिक",
        paksha: "कृष्ण पक्ष",
        tithi: "चतुर्दशी",
        startYear: null,
        endYear: null,
        detail: "तिहारको दोस्रो दिन, कुकुरलाई समर्पित।",
        holiday: true
    },
    {
        event: "लक्ष्मी पूजा",
        eventType: "lunar",
        lunarMonth: "कार्तिक",
        paksha: "कृष्ण पक्ष",
        tithi: "अमावस्या",
        startYear: null,
        endYear: null,
        detail: "तिहारको तेस्रो दिन, देवी लक्ष्मीको पूजा।",
        holiday: true
    },
    {
        event: "गोवर्धन पूजा / म्ह पूजा",
        eventType: "lunar",
        lunarMonth: "कार्तिक",
        paksha: "शुक्ल पक्ष",
        tithi: "प्रतिपदा",
        startYear: null,
        endYear: null,
        detail: "तिहारको चौथो दिन,  आत्म-पूजाको दिन।",
        holiday: true
    },
    {
        event: "भाइटीका",
        eventType: "lunar",
        lunarMonth: "कार्तिक",
        paksha: "शुक्ल पक्ष",
        tithi: "द्वितीया",
        startYear: null,
        endYear: null,
        detail: "तिहारको पाँचौँ दिन, दिदी-भाइबीचको प्रेमको दिन।",
        holiday: true
    },
    {
        event: "छठ पर्व",
        eventType: "lunar",
        lunarMonth: "कार्तिक",
        paksha: "शुक्ल पक्ष",
        tithi: "षष्ठी",
        startYear: null,
        endYear: null,
        detail: "सूर्य देवतालाई समर्पित उत्सव, मुख्यतया तराईमा मनाइन्छ।",
        holiday: true
    },
    {
        event: "हरिबोधिनी एकादशी",
        eventType: "lunar",
        lunarMonth: "कार्तिक",
        paksha: "शुक्ल पक्ष",
        tithi: "एकादशी",
        startYear: null,
        endYear: null,
        detail: "भगवान विष्णुको जागरणको दिन।",
        holiday: false
    },
    {
        event: "विवाह पञ्चमी",
        eventType: "lunar",
        lunarMonth: "मार्गशीर्ष",
        paksha: "शुक्ल पक्ष",
        tithi: "पञ्चमी",
        startYear: null,
        endYear: null,
        detail: "भगवान राम र सीताको विवाहको सम्झना।",
        holiday: true
    },
    {
        event: "य:मरि पुन्हि / धान्यपुर्णिमा",
        eventType: "lunar",
        lunarMonth: "मार्गशीर्ष",
        paksha: "शुक्ल पक्ष",
        tithi: "पूर्णिमा",
        startYear: null,
        endYear: null,
        detail: "नेवार समुदायको यमरि पुन्हि / धान्य पूर्णिमाको उत्सव।",
        holiday: true
    },
    {
        event: "सरस्वती पूजा / वसन्तपञ्चमी",
        eventType: "lunar",
        lunarMonth: "माघ",
        paksha: "शुक्ल पक्ष",
        tithi: "पञ्चमी",
        startYear: null,
        endYear: null,
        detail: "ज्ञान र शिक्षाको देवी सरस्वतीको पूजा।",
        holiday: true
    },
    {
        event: "महाशिवरात्रि",
        eventType: "lunar",
        lunarMonth: "फाल्गुन",
        paksha: "कृष्ण पक्ष",
        tithi: "चतुर्दशी",
        startYear: null,
        endYear: null,
        detail: "भगवान शिवको महान रात्रि।",
        holiday: true
    },
    {
        event: "होली पूर्णिमा",
        eventType: "lunar",
        lunarMonth: "फाल्गुन",
        paksha: "शुक्ल पक्ष",
        tithi: "पूर्णिमा",
        startYear: null,
        endYear: null,
        detail: "रङ्गहरूको उत्सव।",
        holiday: true
    }
];

// Gregorian recurring events.
var gregorianEvents = [
    {
        event: "New Year's Day",
        eventType: "gregorian",
        date: "01/01",
        startYear: null,
        endYear: null,
        detail: "Celebration of the new Gregorian year.",
        holiday: true
    },
    {
        event: "Valentine's Day",
        eventType: "gregorian",
        date: "02/14",
        startYear: null,
        endYear: null,
        detail: "International day of love. Nepali: प्रणय दिवस.",
        holiday: false
    },
    {
        event: "International Women's Day",
        eventType: "gregorian",
        date: "03/08",
        startYear: null,
        endYear: null,
        detail: "Global day celebrating womanhood. Nepali: नारी दिवस.",
        holiday: true
    },
    {
        event: "World Health Day",
        eventType: "gregorian",
        date: "04/07",
        startYear: null,
        endYear: null,
        detail: "A global health awareness day. Nepali: विश्व स्वास्थ्य दिवस.",
        holiday: false
    },
    {
        event: "International Workers' Day",
        eventType: "gregorian",
        date: "05/01",
        startYear: null,
        endYear: null,
        detail: "Also known as May Day. Nepali: अन्तर्राष्ट्रिय श्रमिक दिवस.",
        holiday: true
    },
    {
        event: "World Environment Day",
        eventType: "gregorian",
        date: "06/05",
        startYear: null,
        endYear: null,
        detail: "Day for environmental awareness and action. Nepali: विश्व वातावरण दिवस.",
        holiday: false
    },
    {
        event: "International Day of Yoga",
        eventType: "gregorian",
        date: "06/21",
        startYear: null,
        endYear: null,
        detail: "Global day to celebrate yoga. Nepali: विश्व योग दिवस.",
        holiday: false
    },
    {
        event: "World Population Day",
        eventType: "gregorian",
        date: "07/11",
        startYear: null,
        endYear: null,
        detail: "Day to raise awareness of global population issues. Nepali: विश्व जनसंख्या दिवस.",
        holiday: false
    },
    {
        event: "International Youth Day",
        eventType: "gregorian",
        date: "08/12",
        startYear: null,
        endYear: null,
        detail: "Day to celebrate young people as agents of change. Nepali: अन्तर्राष्ट्रिय युवा दिवस.",
        holiday: false
    },
    {
        event: "International Human Rights Day",
        eventType: "gregorian",
        date: "12/10",
        startYear: null,
        endYear: null,
        detail: "Commemorates the adoption of the Universal Declaration of Human Rights. Nepali: अन्तर्राष्ट्रिय मानव अधिकार दिवस.",
        holiday: false
    },
    {
        event: "Christmas Day",
        eventType: "gregorian",
        date: "12/25",
        startYear: null,
        endYear: null,
        detail: "Christian festival celebrating the birth of Jesus. Nepali: क्रिसमस-डे.",
        holiday: true
    }
];

// Bikram recurring events (based on BS month/day).
var bikramRecurringEvents = [
    {
        event: "लोकतन्त्र दिवस",
        eventType: "bikramRecurring",
        date: "01/11",
        startYear: 2063,
        endYear: null,
        detail: "लोकतन्त्र दिवस, हरेक वर्ष बैशाख ११ गते मनाइन्छ।",
        holiday: true
    },
    {
        event: "गणतन्त्र दिवस",
        eventType: "bikramRecurring",
        date: "02/15",
        startYear: 2065,
        endYear: null,
        detail: "गणतन्त्र दिवस, हरेक वर्ष जेठ १५ गते मनाइन्छ।",
        holiday: true
    },
    {
        event: "राष्ट्रिय धान दिवस",
        eventType: "bikramRecurring",
        date: "03/15",
        startYear: null,
        endYear: null,
        detail: "राष्ट्रिय धान दिवस, हरेक वर्ष असार १५ गते मनाइन्छ。",
        holiday: false
    },
    {
        event: "भानु जयन्ती",
        eventType: "bikramRecurring",
        date: "03/29",
        startYear: null,
        endYear: null,
        detail: "कवि भानुभक्त आचार्यको जन्म जयन्ती।",
        holiday: false
    },
    {
        event: "संविधान दिवस",
        eventType: "bikramRecurring",
        date: "06/03",
        startYear: 2072,
        endYear: null,
        detail: "संविधान दिवस, हरेक वर्ष असोज ३ गते मनाइन्छ।",
        holiday: true
    },
    {
        event: "पृथ्वी जयन्ती / राष्ट्रिय एकता दिवस",
        eventType: "bikramRecurring",
        date: "09/27",
        startYear: null,
        endYear: null,
        detail: "पृथ्वी जयन्ती / राष्ट्रिय एकता दिवस।",
        holiday: true
    },
    {
        event: "शहीद दिवस",
        eventType: "bikramRecurring",
        date: "10/16",
        startYear: null,
        endYear: null,
        detail: "शहीद दिवस।",
        holiday: true
    },
    {
        event: "प्रजातन्त्र दिवस",
        eventType: "bikramRecurring",
        date: "11/07",
        startYear: 2007,
        endYear: 2062,
        detail: "राणा शासनको अन्त्यको सम्झनामा मनाइने प्रजातन्त्र दिवस।",
        holiday: true
    },
    {
        event: "नयाँ वर्ष",
        eventType: "bikramRecurring",
        date: "01/01",
        startYear: null,
        endYear: null,
        detail: "नेपाली नयाँ वर्ष।",
        holiday: true
    },
    {
        event: "बिस्का: जात्रा",
        eventType: "bikramRecurring",
        date: "01/01",
        startYear: null,
        endYear: null,
        detail: "भक्तपुरमा मनाइने बिस्का जात्राको उत्सव।",
        holiday: true
    },
    {
        event: "साउने सङ्क्रान्ति",
        eventType: "bikramRecurring",
        date: "04/01",
        startYear: null,
        endYear: null,
        detail: "श्रावण सङ्क्रान्ति, श्रावण महिनाको पहिलो दिन।",
        holiday: false
    },
    {
        event: "माघे संक्रान्ति",
        eventType: "bikramRecurring",
        date: "10/01",
        startYear: null,
        endYear: null,
        detail: "माघे सङ्क्रान्ति, माघ महिनाको पहिलो दिन।",
        holiday: true
    }
];

// Bikram fixed events (specific BS year/month/day).
var bikramFixedEvents = [
    {
        event: "भोटो जात्रा",
        eventType: "bikramFixed",
        date: "2082/02/18",
        startYear: null,
        endYear: null,
        detail: "रातो मच्छिन्द्रनाथ जात्राको समापन दिन।",
        holiday: true
    },
    {
        event: "तमु ल्होसार",
        eventType: "bikramFixed",
        date: "2082/09/15",
        startYear: null,
        endYear: null,
        detail: "गुरुङ समुदायको नयाँ वर्ष।",
        holiday: true
    },
    {
        event: "सोनाम ल्होसार",
        eventType: "bikramFixed",
        date: "2082/10/05",
        startYear: null,
        endYear: null,
        detail: "तमाङ समुदायको नयाँ वर्ष।",
        holiday: true
    },
    {
        event: "ग्याल्पो ल्होछार",
        eventType: "bikramFixed",
        date: "2082/11/06",
        startYear: null,
        endYear: null,
        detail: "शेर्पा समुदायको नयाँ वर्ष।",
        holiday: true
    }
];
