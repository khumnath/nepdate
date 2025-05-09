
import { CalendarEvent } from '@/types/events';
import { BikramDateObj } from '@/utils/bikramConverter';

export interface UpcomingEventProps {
  events: {
    bikramFixedEvents: CalendarEvent[];
    gregorianEvents: CalendarEvent[];
    bikramRecurringEvents: CalendarEvent[];
  };
  currentDate: BikramDateObj;
  useNepaliLanguage: boolean;
  onEventClick: (year: number, month: number, day: number) => void;
  maxEvents?: number;
  // Customization props
  headerBgColor?: string;
  headerTextColor?: string;
  eventItemBgColor?: string;
  eventTextColor?: string;
  eventDateColor?: string;
  eventBorderColor?: string;
  todayBadgeBgColor?: string;
  todayBadgeTextColor?: string;
  futureBadgeBgColor?: string;
  futureBadgeTextColor?: string;
  borderRadius?: string;
  boxShadow?: string;
}

export interface UpcomingEventItem {
  year: number;
  month: number;
  day: number;
  eventText: string;
  eventDetail: string;
  daysRemaining: number;
  englishDate: Date;
  isHoliday: boolean;
}
