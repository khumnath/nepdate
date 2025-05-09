
import { useState, useEffect } from 'react';
import { hasEvents, getAllEventText, getAllEventDetails, isHoliday } from '@/utils/events';
import { BikramDateObj } from '@/utils/bikramConverter';
import { CalendarEvent } from '@/types/events';
import { UpcomingEventItem } from './types';

export const useUpcomingEvents = (
  events: {
    bikramFixedEvents: CalendarEvent[];
    gregorianEvents: CalendarEvent[];
    bikramRecurringEvents: CalendarEvent[];
  }, 
  todayBS: BikramDateObj,
  todayAD: Date,
  useNepaliLanguage: boolean,
  maxEvents: number = 5
) => {
  const [upcomingEvents, setUpcomingEvents] = useState<UpcomingEventItem[]>([]);

  useEffect(() => {
    const findUpcomingEvents = () => {
      const eventsList: UpcomingEventItem[] = [];
      const maxDaysToCheck = 365; // Check up to a year ahead

      // Clone today's date to use as a counter
      let currentBikramYear = todayBS.year;
      let currentBikramMonth = todayBS.month;
      let currentBikramDay = todayBS.day;

      // Loop through the next maxDaysToCheck days
      for (let dayOffset = 0; dayOffset < maxDaysToCheck; dayOffset++) {
        // Set the date to check
        const checkDateObj = new Date(todayAD);
        checkDateObj.setDate(todayAD.getDate() + dayOffset);

        // Get the gregorian date components
        const checkGregorianYear = checkDateObj.getFullYear();
        const checkGregorianMonth = checkDateObj.getMonth() + 1;
        const checkGregorianDay = checkDateObj.getDate();

        // For simplicity in this MVP, we're incrementing day by day
        // A more accurate approach would use proper bikram date conversion for each day
        currentBikramDay++;
        if (currentBikramDay > 32) {
          // Simplified - actual days per month varies
          currentBikramDay = 1;
          currentBikramMonth++;
          if (currentBikramMonth > 12) {
            currentBikramMonth = 1;
            currentBikramYear++;
          }
        }

        // Check if this date has any events
        if (hasEvents(events.bikramFixedEvents, events.gregorianEvents, events.bikramRecurringEvents, currentBikramYear, currentBikramMonth, currentBikramDay, checkGregorianYear, checkGregorianMonth, checkGregorianDay)) {
          // Get the event text and detail
          const eventText = getAllEventText(events.bikramFixedEvents, events.gregorianEvents, events.bikramRecurringEvents, currentBikramYear, currentBikramMonth, currentBikramDay, checkGregorianYear, checkGregorianMonth, checkGregorianDay, useNepaliLanguage);
          const eventDetail = getAllEventDetails(events.bikramFixedEvents, events.gregorianEvents, events.bikramRecurringEvents, currentBikramYear, currentBikramMonth, currentBikramDay, checkGregorianYear, checkGregorianMonth, checkGregorianDay);

          // Check if the event is a holiday
          const isEventHoliday = isHoliday(events.bikramFixedEvents, events.gregorianEvents, events.bikramRecurringEvents, currentBikramYear, currentBikramMonth, currentBikramDay, checkGregorianYear, checkGregorianMonth, checkGregorianDay);
          eventsList.push({
            year: currentBikramYear,
            month: currentBikramMonth,
            day: currentBikramDay,
            eventText,
            eventDetail,
            daysRemaining: dayOffset,
            englishDate: checkDateObj,
            isHoliday: isEventHoliday
          });

          // Break if we've found enough events
          if (eventsList.length >= maxEvents) break;
        }
      }
      
      return eventsList;
    };

    setUpcomingEvents(findUpcomingEvents());
  }, [events, todayBS, todayAD, useNepaliLanguage, maxEvents]);

  return upcomingEvents;
};
