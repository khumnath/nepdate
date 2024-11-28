var bikramBasedEvents = [
    { 
        event: "Dashain", 
        dateType: "bikram", 
        date: "2082/06/14", 
        detail: "Dashain is the biggest festival in Nepal", 
        category: "Cultural" 
    },
    { 
        event: "Tihar", 
        dateType: "bikram", 
        date: "2082/07/01", 
        detail: "Tihar is the festival of lights.", 
        category: "Cultural" 
    }
];

var gregorianBasedEvents = [
    { 
        event: "Christmas", 
        dateType: "gregorian", 
        date: "12/25", 
        detail: "Christmas celebrates the birth of Jesus Christ.", 
        category: "Religious"
    },
    { 
        event: "New Year", 
        dateType: "gregorian", 
        date: "01/01", 
        detail: "New Year's Day marks the beginning of the new year.", 
        category: "Cultural" 
    }
];

var fixedDateEvents = [
    { 
        event: "International Yoga Day", 
        dateType: "fixed", 
        date: "06/21", 
        detail: "Yoga Day promotes the practice of yoga worldwide.", 
        category: "Health",
        startYear: 2080
    },
     { event: "Company Foundation Day", dateType: "bikram", date: "08/15", detail: "Anniversary of the company's foundation.", startYear: 2078, endYear: 2082 }
];