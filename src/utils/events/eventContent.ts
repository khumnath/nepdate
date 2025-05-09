
import { CalendarEvent } from "../../types/events";

// Function to get event text for a date
export function getEventText(
  events: CalendarEvent[], 
  year: number, 
  month: number, 
  day: number, 
  dateType: 'bikram' | 'gregorian' | 'recurring',
  useNepaliLanguage: boolean = true
): string {
  if (!events || events.length === 0) return '';
  
  let eventText = '';
  events.forEach(event => {
    if ((event.startYear && year < event.startYear) || 
        (event.endYear && year > event.endYear) || 
        event.showOnDay === false) {
      return;
    }

    if (dateType === 'bikram') {
      const [eventYear, eventMonth, eventDay] = event.date.split('/').map(Number);
      if (year === eventYear && month === eventMonth && day === eventDay) {
        eventText += useNepaliLanguage ? event.event : (event.eventEn || event.event);
        eventText += ' ';
      }
    } else if (dateType === 'gregorian' || dateType === 'recurring') {
      const [eventMonth, eventDay] = event.date.split('/').map(Number);
      if (month === eventMonth && day === eventDay) {
        eventText += useNepaliLanguage ? event.event : (event.eventEn || event.event);
        eventText += ' ';
      }
    }
  });
  
  return eventText.trim();
}

// Function to get event detail for a date
export function getEventDetail(
  events: CalendarEvent[], 
  year: number, 
  month: number, 
  day: number, 
  dateType: 'bikram' | 'gregorian' | 'recurring',
  useNepaliLanguage: boolean = true
): string {
  if (!events || events.length === 0) return '';
  
  let eventDetail = '';
  events.forEach(event => {
    if ((event.startYear && year < event.startYear) || 
        (event.endYear && year > event.endYear)) {
      return;
    }

    if (dateType === 'bikram') {
      const [eventYear, eventMonth, eventDay] = event.date.split('/').map(Number);
      if (year === eventYear && month === eventMonth && day === eventDay) {
        const detail = useNepaliLanguage ? event.detail : (event.detailEn || event.detail);
        if (detail) {
          eventDetail += detail;
          eventDetail += ' ';
        }
      }
    } else if (dateType === 'gregorian' || dateType === 'recurring') {
      const [eventMonth, eventDay] = event.date.split('/').map(Number);
      if (month === eventMonth && day === eventDay) {
        const detail = useNepaliLanguage ? event.detail : (event.detailEn || event.detail);
        if (detail) {
          eventDetail += detail;
          eventDetail += ' ';
        }
      }
    }
  });
  
  return eventDetail.trim();
}

// Function to get all event text for a date (across all event types)
export function getAllEventText(
  bikramFixedEvents: CalendarEvent[],
  gregorianEvents: CalendarEvent[],
  bikramRecurringEvents: CalendarEvent[],
  bikramYear: number,
  bikramMonth: number,
  bikramDay: number,
  gregorianYear: number,
  gregorianMonth: number,
  gregorianDay: number,
  useNepaliLanguage: boolean = true
): string {
  const texts = [
    getEventText(bikramFixedEvents, bikramYear, bikramMonth, bikramDay, 'bikram', useNepaliLanguage),
    getEventText(gregorianEvents, gregorianYear, gregorianMonth, gregorianDay, 'gregorian', useNepaliLanguage),
    getEventText(bikramRecurringEvents, bikramYear, bikramMonth, bikramDay, 'recurring', useNepaliLanguage)
  ].filter(text => text.length > 0);
  
  return texts.join(', ');
}

// Function to get all event details for a date (across all event types)
export function getAllEventDetails(
  bikramFixedEvents: CalendarEvent[],
  gregorianEvents: CalendarEvent[],
  bikramRecurringEvents: CalendarEvent[],
  bikramYear: number,
  bikramMonth: number,
  bikramDay: number,
  gregorianYear: number,
  gregorianMonth: number,
  gregorianDay: number,
  useNepaliLanguage: boolean = true
): string {
  const details = [
    getEventDetail(bikramFixedEvents, bikramYear, bikramMonth, bikramDay, 'bikram', useNepaliLanguage),
    getEventDetail(gregorianEvents, gregorianYear, gregorianMonth, gregorianDay, 'gregorian', useNepaliLanguage),
    getEventDetail(bikramRecurringEvents, bikramYear, bikramMonth, bikramDay, 'recurring', useNepaliLanguage)
  ].filter(detail => detail.length > 0);
  
  return details.join('\n\n');
}
