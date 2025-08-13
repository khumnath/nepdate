// EventsData.js

// Gregorian events for every year or with specific start/end year ranges.
var gregorianEvents = [
    {
        event: "Event1Greg",
        dateType: "gregorian",
        date: "08/10", // August 10
        detail: "Test event for August 2025.",
        category: "Test",
        startYear: 2025,
        endYear: 2025
    },
    {
        event: "Event2Greg",
        dateType: "gregorian",
        date: "09/05", // September 5
        detail: "Test event for September 2025.",
        category: "Test",
        startYear: 2025,
        endYear: 2025
    },
    {
        event: "Event3GregRecu",
        dateType: "gregorian",
        date: "08/15", // August 15
        detail: "Recurring Gregorian test event.",
        category: "Test"
        // No startYear or endYear means it recurs every year
    },
];

// Bikram recurring events (based on BS month/day) for every year or with specific start/end year ranges.
var bikramRecurringEvents = [
    {
        event: "Event0BikramRecu",
        dateType: "brecurring",
        date: "04/05", // Shrawan 5
        detail: "Recurring test event for Shrawan.",
        category: "Test",
        startYear: 2082,
        endYear: 2085
    },
    {
        event: "Event1BikramRecu",
        dateType: "brecurring",
        date: "04/08", // Shrawan 8
        detail: "Recurring test event for Shrawan.",
        category: "Test",
        startYear: 2070,
        endYear: 2082
    },
    {
        event: "Event2BikramRecu",
        dateType: "brecurring",
        date: "05/10", // Bhadrapad 10
        detail: "Recurring test event for Bhadrapad.",
        category: "Test",
        startYear: 2082,
        endYear: 2089
    }
];

// Bikram fixed events (specific BS year/month/day).
var bikramFixedEvents = [
    {
        event: "Event1BikramFixed",
        dateType: "bikram",
        date: "2082/04/15", // BS Year 2082, Month 4 (Shrawan), Day 15
        detail: "Fixed test event for Shrawan 2082.",
        category: "Test"
    },
    {
        event: "Event2BikramFixed",
        dateType: "bikram",
        date: "2082/05/20", // BS Year 2082, Month 5 (Bhadrapad), Day 20
        detail: "Fixed test event for Bhadrapad 2082.",
        category: "Test"
    }
];
