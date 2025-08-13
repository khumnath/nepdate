// EventsData.js

// Gregorian events for every year or with specific start/end year ranges.
var gregorianEvents = [
    {
        event: "New Year's Day",
        dateType: "gregorian",
        date: "01/01", // January 1
        detail: "Celebration of the new Gregorian year.",
        category: "International Day",
    },
    {
        event: "Valentine's Day",
        dateType: "gregorian",
        date: "02/14", // February 14
        detail: "International day of love. Nepali: प्रणय दिवस.",
        category: "International Day"
    },
    {
        event: "International Women's Day",
        dateType: "gregorian",
        date: "03/08", // March 8
        detail: "Global day celebrating womanhood. Nepali: नारी दिवस.",
        category: "International Day"
    },
    {
        event: "World Health Day",
        dateType: "gregorian",
        date: "04/07", // April 7
        detail: "A global health awareness day. Nepali: विश्व स्वास्थ्य दिवस.",
        category: "International Day"
    },
    {
        event: "International Workers' Day",
        dateType: "gregorian",
        date: "05/01", // May 1
        detail: "Also known as May Day. Nepali: अन्तर्राष्ट्रिय श्रमिक दिवस.",
        category: "International Day"
    },
    {
        event: "World Environment Day",
        dateType: "gregorian",
        date: "06/05", // June 5
        detail: "Day for environmental awareness and action. Nepali: विश्व वातावरण दिवस.",
        category: "International Day"
    },
    {
        event: "International Day of Yoga",
        dateType: "gregorian",
        date: "06/21", // June 21
        detail: "Global day to celebrate yoga. Nepali: विश्व योग दिवस.",
        category: "International Day"
    },
    {
        event: "World Population Day",
        dateType: "gregorian",
        date: "07/11", // July 11
        detail: "Day to raise awareness of global population issues. Nepali: विश्व जनसंख्या दिवस.",
        category: "International Day"
    },
    {
        event: "International Youth Day",
        dateType: "gregorian",
        date: "08/12", // August 12
        detail: "Day to celebrate young people as agents of change. Nepali: अन्तर्राष्ट्रिय युवा दिवस.",
        category: "International Day"
    },
    {
        event: "International Human Rights Day",
        dateType: "gregorian",
        date: "12/10", // December 10
        detail: "Commemorates the adoption of the Universal Declaration of Human Rights. Nepali: अन्तर्राष्ट्रिय मानव अधिकार दिवस.",
        category: "International Day"
    },
    {
        event: "Christmas Day",
        dateType: "gregorian",
        date: "12/25", // December 25
        detail: "Christian festival celebrating the birth of Jesus. Nepali: क्रिसमस-डे.",
        category: "Festival"
    }
];

// Bikram recurring events (based on BS month/day) for every year or with specific start/end year ranges.
var bikramRecurringEvents = [
    {
        event: "लोकतन्त्र दिवस",
        dateType: "brecurring",
        date: "01/11", // Baisakh 11
        detail: "Loktantra Diwas (Democracy Day), celebrated annually on Baisakh 11.",
        category: "National Day"
    },
    {
        event: "गणतन्त्र दिवस",
        dateType: "brecurring",
        date: "02/15", // Jestha 15
        detail: "Gantantra Diwas (Republic Day), celebrated annually on Jestha 15.",
        category: "National Day"
    },
    {
        event: "राष्ट्रिय धान दिवस",
        dateType: "brecurring",
        date: "03/15", // Asar 15
        detail: "National Paddy Day, celebrated annually on Asar 15.",
        category: "National Day"
    },
    {
        event: "भानु जयन्ती",
        dateType: "brecurring",
        date: "03/29", // Asar 29
        detail: "Bhanu Jayanti, birth anniversary of poet Bhanubhakta Acharya.",
        category: "Jayanti"
    },
    {
        event: "संविधान दिवस",
        dateType: "brecurring",
        date: "06/03", // Ashwin 3
        detail: "Constitution Day, celebrated annually on Ashwin 3.",
        category: "National Day"
    },
    {
        event: "पृथ्वी जयन्ती / राष्ट्रिय एकता दिवस",
        dateType: "brecurring",
        date: "09/27", // Poush 27
        detail: "Prithvi Jayanti / National Unity Day.",
        category: "National Day"
    },
    {
        event: "शहीद दिवस",
        dateType: "brecurring",
        date: "10/16", // Magh 16
        detail: "Sahid Diwas (Martyrs' Day).",
        category: "National Day"
    },
    {
        event: "प्रजातन्त्र दिवस",
        dateType: "brecurring",
        date: "11/07", // Falgun 7
        detail: "Prajatantra Diwas (Democracy Day), marking the end of Rana rule.",
        category: "National Day"
    },
            {
                event: "नयाँ वर्ष",
                dateType: "brecurring",
                date: "01/01",
                detail: "Nepali New Year 2082.",
                category: "Festival"
            },
            {
                event: "बिस्का: जात्रा",
                dateType: "brecurring",
                date: "01/01",
                detail: "Biska Jatra festival in Bhaktapur.",
                category: "Jatra"
            },
            {
                event: "साउने सङ्क्रान्ति",
                dateType: "brecurring",
                date: "04/01",
                detail: "Shrawan Sankranti, first day of the month Shrawan.",
                category: "Sankranti"
            },
            {
                event: "माघे संक्रान्ति",
                dateType: "brecurring",
                date: "10/01",
                detail: "Maghe Sankranti, first day of the month Magh.",
                category: "Sankranti"
            },
];

// Bikram fixed events (specific BS year/month/day).
var bikramFixedEvents = [
    {
        event: "मातातीर्थ औंसी",
        dateType: "bikram",
        date: "2082/01/14",
        detail: "Mother's Day.",
        category: "Festival"
    },
    {
        event: "अक्षय तृतीया",
        dateType: "bikram",
        date: "2082/01/17",
        detail: "Akshaya Tritiya festival.",
        category: "Festival"
    },
    {
        event: "बुद्ध जयन्ती / उभौली पर्व",
        dateType: "bikram",
        date: "2082/01/29",
        detail: "Buddha's Birthday and Kirat festival Ubhauli.",
        category: "Festival"
    },
    {
        event: "भोटो जात्रा",
        dateType: "bikram",
        date: "2082/02/18",
        detail: "The concluding day of Rato Machindranath Jatra.",
        category: "Jatra"
    },
    {
        event: "गुरु पुर्णिमा",
        dateType: "bikram",
        date: "2082/03/26",
        detail: "Guru Purnima, a day to honor teachers.",
        category: "Festival"
    },
    {
        event: "नाग पञ्चमी",
        dateType: "bikram",
        date: "2082/04/13",
        detail: "Day of worshipping the serpent deities (Nagas).",
        category: "Festival"
    },
    {
        event: "जनै पूर्णिमा / रक्षा बन्धन",
        dateType: "bikram",
        date: "2082/04/24",
        detail: "The sacred thread festival.",
        category: "Festival"
    },
    {
        event: "गाईजात्रा",
        dateType: "bikram",
        date: "2082/04/25",
        detail: "Festival of cows, a carnival of humor and satire.",
        category: "Jatra"
    },
    {
        event: "श्रीकृष्ण जन्माष्टमी",
        dateType: "bikram",
        date: "2082/04/31",
        detail: "Lord Krishna's birthday.",
        category: "Jayanti"
    },
    {
        event: "कुशे औंसी (बुबाको मुख हेर्ने दिन)",
        dateType: "bikram",
        date: "2082/05/07",
        detail: "Kushe Aunsi, Father's Day.",
        category: "Festival"
    },
    {
        event: "हरितालिका तीज",
        dateType: "bikram",
        date: "2082/05/10",
        detail: "A major festival for Hindu women.",
        category: "Festival"
    },
    {
        event: "इन्द्रजात्रा",
        dateType: "bikram",
        date: "2082/05/21",
        detail: "Major street festival in Kathmandu.",
        category: "Jatra"
    },
    {
        event: "घटस्थापना",
        dateType: "bikram",
        date: "2082/06/06",
        detail: "Marks the beginning of the Dashain festival.",
        category: "Festival"
    },
    {
        event: "फूलपाती",
        dateType: "bikram",
        date: "2082/06/13",
        detail: "The 7th day of Dashain.",
        category: "Festival"
    },
    {
        event: "महाअष्टमी / कालरात्रि",
        dateType: "bikram",
        date: "2082/06/14",
        detail: "The 8th day of Dashain, involving sacrifices.",
        category: "Festival"
    },
    {
        event: "महानवमी",
        dateType: "bikram",
        date: "2082/06/15",
        detail: "The 9th day of Dashain.",
        category: "Festival"
    },
    {
        event: "विजया दशमी",
        dateType: "bikram",
        date: "2082/06/16",
        detail: "The 10th day of Dashain, the main day of celebration.",
        category: "Festival"
    },
    {
        event: "कोजाग्रत पूर्णिमा",
        dateType: "bikram",
        date: "2082/06/20",
        detail: "The full moon day that marks the end of Dashain.",
        category: "Festival"
    },
    {
        event: "लक्ष्मी पूजा / कुकुर तिहार",
        dateType: "bikram",
        date: "2082/07/03",
        detail: "Day for worshipping Goddess Laxmi and dogs.",
        category: "Festival"
    },
    {
        event: "गाई पूजा / गोवर्धन पूजा / म्हपूजा",
        dateType: "bikram",
        date: "2082/07/05",
        detail: "Day for worshipping cows, oxen, and oneself (Mha Puja).",
        category: "Festival"
    },
    {
        event: "भाइटीका",
        dateType: "bikram",
        date: "2082/07/06",
        detail: "The final and main day of Tihar, celebrating siblings.",
        category: "Festival"
    },
    {
        event: "छठ पर्व",
        dateType: "bikram",
        date: "2082/07/10",
        detail: "Festival dedicated to the Sun God, mainly in Terai.",
        category: "Festival"
    },
    {
        event: "विवाह पञ्चमी",
        dateType: "bikram",
        date: "2082/08/09",
        detail: "Commemorates the wedding of Lord Ram and Sita.",
        category: "Festival"
    },
    {
        event: "य:मरि पुन्हि / धान्यपुर्णिमा",
        dateType: "bikram",
        date: "2082/08/18",
        detail: "Newari festival of Yamari Punhi / Dhanya Purnima.",
        category: "Festival"
    },
    {
        event: "तमु ल्होसार",
        dateType: "bikram",
        date: "2082/09/15",
        detail: "New Year of the Gurung community.",
        category: "Lhosar"
    },
    {
        event: "सोनाम ल्होसार",
        dateType: "bikram",
        date: "2082/10/05",
        detail: "New Year of the Tamang community.",
        category: "Lhosar"
    },
    {
        event: "सरस्वती पूजा / वसन्तपञ्चमी",
        dateType: "bikram",
        date: "2082/10/09",
        detail: "Saraswati Puja / Vasant Panchami, day of learning.",
        category: "Festival"
    },
    {
        event: "महा शिवरात्री / नेपाली सेना दिवस",
        dateType: "bikram",
        date: "2082/11/04",
        detail: "Festival of Lord Shiva and Nepali Army Day.",
        category: "Festival"
    },
    {
        event: "ग्याल्पो ल्होछार",
        dateType: "bikram",
        date: "2082/11/06",
        detail: "New Year of the Sherpa community.",
        category: "Lhosar"
    },
    {
        event: "फागु पुर्णिमा / होली",
        dateType: "bikram",
        date: "2082/11/18",
        detail: "Holi, the festival of colors.",
        category: "Festival"
    },
    {
        event: "घोडेजात्रा",
        dateType: "bikram",
        date: "2082/12/04",
        detail: "Ghode Jatra, the horse parade festival in Kathmandu.",
        category: "Jatra"
    },
    {
        event: "चैते दशैँ",
        dateType: "bikram",
        date: "2082/12/12",
        detail: "Chaite Dashain, a smaller version of the great Dashain.",
        category: "Festival"
    },
    {
        event: "श्री राम नवमी",
        dateType: "bikram",
        date: "2082/12/13",
        detail: "Celebration of Lord Ram's birthday.",
        category: "Festival"
    }
];
