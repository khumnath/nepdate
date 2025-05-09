
import { CalendarEvent } from "../../types/events";

// Function to check if a date has an event
export function checkEvent(
  events: CalendarEvent[], 
  year: number, 
  month: number, 
  day: number, 
  dateType: 'bikram' | 'gregorian' | 'recurring'
): boolean {
  if (!events || events.length === 0) return false;
  
  let hasEvent = false;
  events.forEach(event => {
    // Skip events that are outside of the specified year range
    if ((event.startYear && year < event.startYear) || 
        (event.endYear && year > event.endYear) || 
        event.showOnDay === false) {
      return;
    }

    if (dateType === 'bikram') {
      const [eventYear, eventMonth, eventDay] = event.date.split('/').map(Number);
      if (year === eventYear && month === eventMonth && day === eventDay) {
        hasEvent = true;
      }
    } else if (dateType === 'gregorian' || dateType === 'recurring') {
      const [eventMonth, eventDay] = event.date.split('/').map(Number);
      if (month === eventMonth && day === eventDay) {
        hasEvent = true;
      }
    }
  });
  
  return hasEvent;
}

// Function to check if a date has any events (across all event types)
export function hasEvents(
  bikramFixedEvents: CalendarEvent[],
  gregorianEvents: CalendarEvent[],
  bikramRecurringEvents: CalendarEvent[],
  bikramYear: number,
  bikramMonth: number,
  bikramDay: number,
  gregorianYear: number,
  gregorianMonth: number,
  gregorianDay: number
): boolean {
  return (
    checkEvent(bikramFixedEvents, bikramYear, bikramMonth, bikramDay, 'bikram') ||
    checkEvent(gregorianEvents, gregorianYear, gregorianMonth, gregorianDay, 'gregorian') ||
    checkEvent(bikramRecurringEvents, bikramYear, bikramMonth, bikramDay, 'recurring')
  );
}

// Function to check if a date is a holiday
export function isHoliday(
  bikramFixedEvents: CalendarEvent[],
  gregorianEvents: CalendarEvent[],
  bikramRecurringEvents: CalendarEvent[],
  bikramYear: number,
  bikramMonth: number,
  bikramDay: number,
  gregorianYear: number,
  gregorianMonth: number,
  gregorianDay: number
): boolean {
  let holiday = false;
  
  // Check bikram fixed events
  bikramFixedEvents.forEach(event => {
    if ((event.startYear && bikramYear < event.startYear) || 
        (event.endYear && bikramYear > event.endYear)) {
      return;
    }
    const [eventYear, eventMonth, eventDay] = event.date.split('/').map(Number);
    if (bikramYear === eventYear && bikramMonth === eventMonth && bikramDay === eventDay && event.isHoliday) {
      holiday = true;
    }
  });
  
  // If not found in bikram events, check gregorian events
  if (!holiday) {
    gregorianEvents.forEach(event => {
      if ((event.startYear && gregorianYear < event.startYear) || 
          (event.endYear && gregorianYear > event.endYear)) {
        return;
      }
      const [eventMonth, eventDay] = event.date.split('/').map(Number);
      if (gregorianMonth === eventMonth && gregorianDay === eventDay && event.isHoliday) {
        holiday = true;
      }
    });
  }
  
  // If not found in bikram recurring events, check bikram recurring events
  if (!holiday) {
    bikramRecurringEvents.forEach(event => {
      if ((event.startYear && bikramYear < event.startYear) || 
          (event.endYear && bikramYear > event.endYear)) {
        return;
      }
      const [eventMonth, eventDay] = event.date.split('/').map(Number);
      if (bikramMonth === eventMonth && bikramDay === eventDay && event.isHoliday) {
        holiday = true;
      }
    });
  }
  
  return holiday;
}
