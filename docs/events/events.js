// use this file for events that need every year or can be applied start and end year.

// gregorian month/day for every year or put start, end or both values for date range.
var gregorianEvents = [
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
    },
// test data
    { 
        event: "test gregorian event", 
        dateType: "gregorian", 
        date: "12/05", 
        detail: "this is event test.which only shows in 2024.12.05 AD", 
        category: "test", // catagory is not implemented. we can use for isHoliday to mark day as holiday
        startYear: 2024,
        endYear: 2024
    },
];

// bikram events for every year or put start, end or both values for date range.
var bikramRecurringEvents = [
    { 
        event: "International Yoga Day", 
        dateType: "brecurring", 
        date: "06/21", 
        detail: "Yoga Day promotes the practice of yoga worldwide.", 
        category: "Health",
        startYear: 2080
    },
     { 
        event: "Company Foundation Day",
        dateType: "brecurring",
        date: "08/15",
        detail: "Anniversary of the company's foundation. BS 2078.8.15 to 2080.08/15 only",
        startYear: 2078,
        endYear: 2082
     }
];



var bikramFixedEvents = [
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
    },
// test data
    { 
        event: "test bikram event", 
        dateType: "bikram", 
        date: "2081/08/19", 
        detail: "this is event detail. this event has fixed date", 
        category: "Cultural" 
    }
];