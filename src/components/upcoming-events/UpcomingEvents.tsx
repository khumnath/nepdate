
import React from 'react';
import EventsHeader from './EventsHeader';
import EventItem from './EventItem';
import { useUpcomingEvents } from './useUpcomingEvents';
import { UpcomingEventProps } from './types';

const UpcomingEvents: React.FC<UpcomingEventProps> = ({
  events,
  currentDate,
  useNepaliLanguage,
  onEventClick,
  maxEvents = 5,
  // Default values for customization props
  headerBgColor = "bg-gradient-to-r from-blue-800 to-blue-600",
  headerTextColor = "text-white",
  eventItemBgColor = "bg-white",
  eventTextColor = "text-blue-500",
  eventDateColor = "text-gray-500",
  eventBorderColor = "border-gray-200",
  todayBadgeBgColor = "bg-green-100",
  todayBadgeTextColor = "text-green-800",
  futureBadgeBgColor = "bg-blue-100",
  futureBadgeTextColor = "text-blue-800",
  borderRadius = "rounded-lg",
  boxShadow = "shadow-md",
}) => {
  // Get today's date objects
  const todayBS = currentDate;
  const todayAD = currentDate.englishDate;
  
  // Use the custom hook to get upcoming events
  const upcomingEvents = useUpcomingEvents(
    events, 
    todayBS, 
    todayAD, 
    useNepaliLanguage, 
    maxEvents
  );

  if (upcomingEvents.length === 0) {
    return null;
  }

  return (
    <div className={`mt-4 sm:mt-6 ${eventItemBgColor} ${borderRadius} border border-gray-300 overflow-hidden beautiful-card no-print ${boxShadow} font-mukta`}>
      <EventsHeader 
        headerBgColor={headerBgColor}
        headerTextColor={headerTextColor}
        useNepaliLanguage={useNepaliLanguage}
      />
      <div className={`divide-y ${eventBorderColor}`}>
        {upcomingEvents.map((event, index) => (
          <EventItem 
            key={index} 
            event={event} 
            index={index} 
            useNepaliLanguage={useNepaliLanguage} 
            eventItemBgColor={eventItemBgColor} 
            eventTextColor={eventTextColor} 
            eventDateColor={eventDateColor} 
            todayBadgeBgColor={todayBadgeBgColor} 
            todayBadgeTextColor={todayBadgeTextColor} 
            futureBadgeBgColor={futureBadgeBgColor} 
            futureBadgeTextColor={futureBadgeTextColor} 
            onEventClick={onEventClick} 
          />
        ))}
      </div>
    </div>
  );
};

export default UpcomingEvents;
