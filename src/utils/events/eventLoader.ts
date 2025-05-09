
import { CalendarEvent } from "../../types/events";
import { defaultEvents } from "./types";

// Function to load events for a specific year
export async function loadEventsForYear(year: number): Promise<{
  bikramFixedEvents: CalendarEvent[];
  gregorianEvents: CalendarEvent[];
  bikramRecurringEvents: CalendarEvent[];
}> {
  try {
    // First try to load year-specific events
    const yearModule = await import(`../../data/events-${year}.ts`).catch(() => {
      console.log(`No specific events for year ${year}, using default events`);
      return { default: defaultEvents };
    });
    
    // Then try to load common events
    const commonModule = await import('../../data/events.ts').catch(() => {
      console.log('No common events found');
      return { default: { 
        bikramFixedEvents: [], 
        gregorianEvents: [], 
        bikramRecurringEvents: [] 
      }};
    });
    
    // Filter events based on startYear and endYear
    const filterByYearRange = (events: CalendarEvent[]): CalendarEvent[] => {
      return events.filter(event => 
        (!event.startYear || year >= event.startYear) && 
        (!event.endYear || year <= event.endYear)
      );
    };
    
    // Merge year-specific and common events
    return {
      bikramFixedEvents: [
        ...filterByYearRange(commonModule.default.bikramFixedEvents || []),
        ...filterByYearRange(yearModule.default.bikramFixedEvents || [])
      ],
      gregorianEvents: [
        ...filterByYearRange(commonModule.default.gregorianEvents || []),
        ...filterByYearRange(yearModule.default.gregorianEvents || [])
      ],
      bikramRecurringEvents: [
        ...filterByYearRange(commonModule.default.bikramRecurringEvents || []),
        ...filterByYearRange(yearModule.default.bikramRecurringEvents || [])
      ]
    };
  } catch (error) {
    console.error('Error loading events:', error);
    return defaultEvents;
  }
}
