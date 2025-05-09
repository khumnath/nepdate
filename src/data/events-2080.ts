
import { CalendarEvent } from "../types/events";

// if isHoliday is true, the event is considered a holiday.
// if isHoliday is false, the event is not considered a holiday.
// date is in the format "YYYY/MM/DD" for fixed events.

// Events specific to the year 2080 BS
const bikramFixedEvents: CalendarEvent[] = [
  {
    date: "2080/10/15",
    event: "माघ पूर्णिमा",
    eventEn: "Maghe Purnima",
    detail: "२०८० बिक्रम संवत्, माघ पूर्णिमा",
    detailEn: "Full moon day of Magh month, 2080 BS",
    type: "bikram",
    isHoliday: true
  },
  {
    date: "2080/11/9",
    event: "महाशिवरात्री",
    eventEn: "Maha Shivaratri",
    detail: "शिवको पूजा गर्ने पवित्र दिन",
    detailEn: "Sacred day dedicated to Lord Shiva",
    type: "bikram",
    isHoliday: true
  },
  {
    date: "2080/12/30",
    event: "वार्षिक समारोह",
    eventEn: "Annual Celebration",
    detail: "विशेष समारोह",
    detailEn: "Special celebration  2080 BS",
    type: "bikram"
  }
];

export default {
  bikramFixedEvents,
  bikramRecurringEvents: []
};
