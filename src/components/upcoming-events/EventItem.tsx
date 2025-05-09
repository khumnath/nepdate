
import React from 'react';
import { Calendar as CalendarIcon } from 'lucide-react';
import { Button } from '@/components/ui/button';
import { Card } from '@/components/ui/card';
import { UpcomingEventItem } from './types';
import { formatDate, formatDaysRemaining, getEventItemLeftBorderColor } from './utils';

interface EventItemProps {
  event: UpcomingEventItem;
  index: number;
  useNepaliLanguage: boolean;
  eventItemBgColor: string;
  eventTextColor: string;
  eventDateColor: string;
  todayBadgeBgColor: string;
  todayBadgeTextColor: string;
  futureBadgeBgColor: string;
  futureBadgeTextColor: string;
  onEventClick: (year: number, month: number, day: number) => void;
}

const EventItem: React.FC<EventItemProps> = ({
  event,
  index,
  useNepaliLanguage,
  eventItemBgColor,
  eventTextColor,
  eventDateColor,
  todayBadgeBgColor,
  todayBadgeTextColor,
  futureBadgeBgColor,
  futureBadgeTextColor,
  onEventClick,
}) => {
  return (
    <Card 
      className={`p-0 border-0 rounded-none ${getEventItemLeftBorderColor(index, event.isHoliday)} animated-hover`}
    >
      <Button 
        variant="ghost" 
        onClick={() => onEventClick(event.year, event.month, event.day)} 
        className={`w-full flex items-start p-3 h-auto justify-between text-center text-sm font-light ${eventItemBgColor}`}
      >
        <div className="flex flex-col">
          <span className={`font-bold text-sm ${event.isHoliday ? 'text-red-600' : eventTextColor} upcoming-event-text font-mukta`}>
            {event.eventText}
          </span>
          <span className={`text-xs ${eventDateColor} font-mukta`}>
            {formatDate(event.month, event.day, event.year, useNepaliLanguage)}
          </span>
        </div>
        <div className="flex items-center gap-1">
          <span className={`text-xs ${
            event.daysRemaining === 0 
              ? `${todayBadgeBgColor} ${todayBadgeTextColor}` 
              : `${futureBadgeBgColor} ${futureBadgeTextColor}`
          } py-1 px-2 rounded font-mukta`}>
            {formatDaysRemaining(event.daysRemaining, useNepaliLanguage)}
          </span>
          <CalendarIcon className="h-4 w-4 text-gray-500" />
        </div>
      </Button>
    </Card>
  );
};

export default EventItem;
