import { useState, useEffect } from 'react';
import { hasEvents, getAllEventText, getAllEventDetails, isHoliday } from '@/utils/events';
import { BikramDateObj } from '@/utils/bikramConverter';
import { CalendarEvent } from '@/types/events';
import { UpcomingEventItem } from './types';
import { BikramDate } from '@/utils/bikram';

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

      // Loop through the next maxDaysToCheck days
      for (let dayOffset = 0; dayOffset < maxDaysToCheck; dayOffset++) {
        // Set the date to check
        const checkDateObj = new Date(todayAD);
        checkDateObj.setDate(todayAD.getDate() + dayOffset);

        // Get the gregorian date components
        const checkGregorianYear = checkDateObj.getFullYear();
        const checkGregorianMonth = checkDateObj.getMonth() + 1;
        const checkGregorianDay = checkDateObj.getDate();

        // Convert Gregorian date to Bikram date for this day
        const bikramDateObj = new BikramDate().fromGregorian(
          checkGregorianYear,
          checkGregorianMonth,
          checkGregorianDay
        );
        const currentBikramYear = bikramDateObj.bsYear;
        const currentBikramMonth = bikramDateObj.bsMonth;
        const currentBikramDay = bikramDateObj.bsDay;

        // Check if this date has any events
        if (hasEvents(
          events.bikramFixedEvents,
          events.gregorianEvents,
          events.bikramRecurringEvents,
          currentBikramYear,
          currentBikramMonth,
          currentBikramDay,
          checkGregorianYear,
          checkGregorianMonth,
          checkGregorianDay
        )) {
          // Get the event text and detail
          const eventText = getAllEventText(
            events.bikramFixedEvents,
            events.gregorianEvents,
            events.bikramRecurringEvents,
            currentBikramYear,
            currentBikramMonth,
            currentBikramDay,
            checkGregorianYear,
            checkGregorianMonth,
            checkGregorianDay,
            useNepaliLanguage
          );
          const eventDetail = getAllEventDetails(
            events.bikramFixedEvents,
            events.gregorianEvents,
            events.bikramRecurringEvents,
            currentBikramYear,
            currentBikramMonth,
            currentBikramDay,
            checkGregorianYear,
            checkGregorianMonth,
            checkGregorianDay
          );

          // Check if the event is a holiday
          const isEventHoliday = isHoliday(
            events.bikramFixedEvents,
            events.gregorianEvents,
            events.bikramRecurringEvents,
            currentBikramYear,
            currentBikramMonth,
            currentBikramDay,
            checkGregorianYear,
            checkGregorianMonth,
            checkGregorianDay
          );
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