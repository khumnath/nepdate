
import { CalendarEvent } from "../../types/events";

// Default events (empty arrays)
export const defaultEvents: {
  bikramFixedEvents: CalendarEvent[];
  gregorianEvents: CalendarEvent[];
  bikramRecurringEvents: CalendarEvent[];
} = {
  bikramFixedEvents: [],
  gregorianEvents: [],
  bikramRecurringEvents: []
};
