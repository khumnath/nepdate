import React from 'react';
import { nepaliDaysEn, nepaliDaysNp, getNepaliDigits } from '../utils/bikramConverter';
import { calculateTithi } from '../utils/tithiCalculation';
import { CalendarEvent } from '../types/events';
import { hasEvents, getAllEventText, getAllEventDetails, isHoliday } from '../utils/events';

interface CalendarGridProps {
  year: number;
  month: number; // 1-12
  days: number;
  startWeekDay: number; // 0-6, where 0 is Sunday
  currentDate?: {
    year: number;
    month: number;
    day: number;
  };
  selectedDate?: {
    year: number;
    month: number;
    day: number;
  };
  englishStartDate: Date;
  onDateSelect: (day: number, source?: "calendar" | "converter") => void;
  useNepaliLanguage: boolean;
  events?: {
    bikramFixedEvents: CalendarEvent[];
    gregorianEvents: CalendarEvent[];
    bikramRecurringEvents: CalendarEvent[];
  };
  onEventClick?: (eventData: {
    day: number;
    tithiName: string;
    tithiPaksha: string;
    englishDate: Date;
    eventText: string;
    eventDetail: string;
  }) => void;
  usingApproximation?: boolean;
}

const CalendarGrid: React.FC<CalendarGridProps> = ({
  year,
  month,
  days,
  startWeekDay,
  currentDate,
  selectedDate,
  englishStartDate,
  onDateSelect,
  useNepaliLanguage,
  events = {
    bikramFixedEvents: [],
    gregorianEvents: [],
    bikramRecurringEvents: []
  },
  onEventClick,
  usingApproximation = false
}) => {
  // Generate the days array
  const daysArray = [];
  const totalSlots = Math.ceil((days + startWeekDay) / 7) * 7;

  // Add empty slots for days before the 1st of month
  for (let i = 0; i < startWeekDay; i++) {
    daysArray.push(null);
  }

  // Add the days of the month
  for (let i = 1; i <= days; i++) {
    daysArray.push(i);
  }

  // Add empty slots for days after the end of month
  for (let i = days + startWeekDay; i < totalSlots; i++) {
    daysArray.push(null);
  }

  // Function to handle the day click
  const handleDayClick = (day: number | null, e: React.MouseEvent) => {
    e.preventDefault();
    e.stopPropagation();

    if (day === null) return;

    onDateSelect(day, "calendar");

    // Get English date for this day
    const englishDate = getEnglishDate(day);

    // Get tithi for this day
    const tithi = getTithiForDay(day);

    // Check for events
    const dayHasEvents = hasEvents(
      events.bikramFixedEvents,
      events.gregorianEvents,
      events.bikramRecurringEvents,
      year, month, day,
      englishDate.getFullYear(),
      englishDate.getMonth() + 1,
      englishDate.getDate()
    );

    // Always call onEventClick for any day, whether it has events or not
    if (onEventClick) {
      const eventText = dayHasEvents ? getAllEventText(
        events.bikramFixedEvents,
        events.gregorianEvents,
        events.bikramRecurringEvents,
        year, month, day,
        englishDate.getFullYear(),
        englishDate.getMonth() + 1,
        englishDate.getDate(),
        useNepaliLanguage
      ) : '';

      const eventDetail = dayHasEvents ? getAllEventDetails(
        events.bikramFixedEvents,
        events.gregorianEvents,
        events.bikramRecurringEvents,
        year, month, day,
        englishDate.getFullYear(),
        englishDate.getMonth() + 1,
        englishDate.getDate(),
        useNepaliLanguage
      ) : '';

      onEventClick({
        day,
        tithiName: useNepaliLanguage ? tithi.name : tithi.nameEn,
        tithiPaksha: useNepaliLanguage ? tithi.paksha : tithi.pakshaEn,
        englishDate,
        eventText,
        eventDetail
      });
    }
  };

  // Split into weeks
  const weeks = [];
  for (let i = 0; i < daysArray.length; i += 7) {
    weeks.push(daysArray.slice(i, i + 7));
  }
  const weekDays = useNepaliLanguage ? nepaliDaysNp : nepaliDaysEn;

  // Function to get English date for a specific Bikram day
  const getEnglishDate = (day: number): Date => {
    // Clone the start date and add days
    const dayDate = new Date(englishStartDate);
    dayDate.setDate(englishStartDate.getDate() + (day - 1));
    return dayDate;
  };

  // Function to get tithi for a given day
  const getTithiForDay = (day: number): {
    name: string;
    nameEn: string;
    paksha: string;
    pakshaEn: string;
  } => {
    if (!day) return {
      name: '',
      nameEn: '',
      paksha: '',
      pakshaEn: ''
    };
    const englishDate = getEnglishDate(day);
    const tithiData = calculateTithi(englishDate);
    return {
      name: tithiData.tithiName,
      nameEn: tithiData.tithiNameEn,
      paksha: tithiData.paksha,
      pakshaEn: tithiData.pakshaEn
    };
  };

  return (
    <div className="beautiful-calendar-grid overflow-hidden shadow-md">
      {/* Days of week header */}
      <div className="grid grid-cols-7 overflow-hidden">
        {weekDays.map((day, idx) => (
          <div key={idx} className={`text-center py-1 sm:py-2 px-1 font-bold text-xs sm:text-sm ${idx === 0 ? 'bg-blue-800 text-white' : idx === 6 ? 'bg-red-700 text-white' : 'bg-blue-700 text-white'}`}>
            <span className={useNepaliLanguage ? "font-laila" : ""}>
              {day}
            </span>
            <div className="text-[7px] sm:text-xs font-normal hidden sm:block">
              {useNepaliLanguage ? '' : idx === 0 ? 'Sunday' : idx === 1 ? 'Monday' : idx === 2 ? 'Tuesday' : idx === 3 ? 'Wednesday' : idx === 4 ? 'Thursday' : idx === 5 ? 'Friday' : 'Saturday'}
            </div>
          </div>
        ))}
      </div>

      {/* Calendar grid */}
      <div className="bg-white grid grid-cols-7 divide-x divide-y divide-gray-300">
        {weeks.map((week, weekIdx) => (
          <React.Fragment key={weekIdx}>
            {week.map((day, dayIdx) => {
              // Check if this is current day, selected day
              const isCurrentDay = currentDate && day === currentDate.day && month === currentDate.month && year === currentDate.year;
              const isSelectedDay = selectedDate && day === selectedDate.day && month === selectedDate.month && year === selectedDate.year;

              // Whether it's a weekend (Saturday or Sunday)
              const isSaturday = dayIdx === 6;
              const isSunday = dayIdx === 0;

              if (day === null) {
                return <div key={dayIdx} className="min-h-[50px] sm:min-h-[80px] relative p-0 sm:p-0.5 md:p-1 bg-gray-100"></div>;
              }

              // Get English date for this day
              const englishDate = getEnglishDate(day);
              const englishDay = englishDate.getDate();
              const englishMonth = englishDate.getMonth() + 1;
              const englishYear = englishDate.getFullYear();

              // Get tithi for this day
              const tithi = getTithiForDay(day);

              // Format English date
              const englishDateStr = englishDate.getDate() === 1 ? englishDate.toLocaleDateString('en-US', {
                month: 'short',
                day: 'numeric'
              }) : englishDate.getDate().toString();

              // Check for events
              const dayHasEvents = hasEvents(events.bikramFixedEvents, events.gregorianEvents, events.bikramRecurringEvents, year, month, day, englishYear, englishMonth, englishDay);

              // Check if this day is a holiday
              const dayIsHoliday = isHoliday(events.bikramFixedEvents, events.gregorianEvents, events.bikramRecurringEvents, year, month, day, englishYear, englishMonth, englishDay);

              // Get event text if there's an event
              const eventText = dayHasEvents ? getAllEventText(events.bikramFixedEvents, events.gregorianEvents, events.bikramRecurringEvents, year, month, day, englishYear, englishMonth, englishDay, useNepaliLanguage) : '';

              // Get event detail if there's an event
              const eventDetail = dayHasEvents ? getAllEventDetails(events.bikramFixedEvents, events.gregorianEvents, events.bikramRecurringEvents, year, month, day, englishYear, englishMonth, englishDay, useNepaliLanguage) : '';

              // Special rendering for purnima and amavasya
              const isPurnima = tithi.nameEn === 'Purnima';
              const isAmavasya = tithi.nameEn === 'Amavasya';
              const specialTithi = isPurnima || isAmavasya;

              return (
                <div
                  key={dayIdx}
                  className={`min-h-[50px] sm:min-h-[80px] relative p-0 sm:p-0.5 md:p-1 
                    cursor-pointer hover:bg-gray-50 beautiful-calendar-day
                    ${isSaturday ? 'bg-red-50' : isSunday ? 'bg-blue-50' : ''} 
                    ${isCurrentDay ? 'bg-yellow-50' : ''}`}
                  onClick={(e) => handleDayClick(day, e)}
                  role="button"
                  tabIndex={0}
                  aria-label={`Day ${day}`}
                  onKeyDown={(e) => {
                    if (e.key === 'Enter' || e.key === ' ') {
                      handleDayClick(day, e as unknown as React.MouseEvent);
                    }
                  }}
                >
                  <div className="h-full relative rounded-none">
                    <div className={`flex justify-between ${isSelectedDay ? 'bg-green-300 rounded-sm' : ''}`}>
                      <div className="flex flex-col items-start p-0.5 sm:p-1">
                        {/* Bikram date - larger and bold */}
                        <span className={`text-xl sm:text-2xl lg:text-3xl font-bold 
                          ${dayIsHoliday ? 'text-red-600' : isSaturday ? 'text-red-600' : isSunday ? 'text-blue-700' : 'text-gray-800'}
                          ${useNepaliLanguage ? "font-laila" : ""}
                          ${isCurrentDay ? 'ring-1 ring-red-500 px-0.5 sm:px-1 rounded-full' : ''}`}>
                          {useNepaliLanguage ? getNepaliDigits(day) : day}
                        </span>
                      </div>

                      {/* English date - smaller and lighter */}
                      <span className="text-[6px] sm:text-[8px] md:text-xs text-gray-500 p-0.5">
                        {englishDateStr}
                      </span>
                    </div>

                    {/* Event display */}
                    {dayHasEvents && (
                      <div className="text-[7px] sm:text-[8px] md:text-xs text-red-800 rounded-sm mt-0.5 block truncate w-full text-center px-0 py-0 my-0 bg-rose-50">
                        <span className={useNepaliLanguage ? "font-laila" : ""}>
                          {eventText}
                        </span>
                      </div>
                    )}

                    {/* Special tithi display (purnima/amavasya) */}
                    {specialTithi && !dayHasEvents && (
                      <div className="text-[7px] sm:text-[8px] md:text-xs px-1 py-0.5 text-yellow-800 rounded-sm mt-0.5 block truncate w-full text-center bg-transparent">
                        <span className={useNepaliLanguage ? "font-laila" : ""}>
                          {useNepaliLanguage ? tithi.name : tithi.nameEn}
                        </span>
                      </div>
                    )}

                    {/* Regular tithi display */}
                    {!specialTithi && !dayHasEvents && (
                      <div className="text-[7px] sm:text-[8px] md:text-[10px] text-blue-600 mt-0.5 px-1">
                        <span className={useNepaliLanguage ? "font-laila" : ""}>
                          {useNepaliLanguage ? tithi.name : tithi.nameEn}
                        </span>
                      </div>
                    )}
                  </div>
                </div>
              );
            })}
          </React.Fragment>
        ))}
      </div>

      {/* Approximation notice */}
      {usingApproximation && (
        <div className="bg-yellow-50 text-yellow-800 text-xs p-2 text-center border-t border-yellow-200">
          <span className={useNepaliLanguage ? "font-laila" : ""}>
            {useNepaliLanguage ? 'यो मिति प्रारम्भिक गणनाबाट प्राप्त गरिएको हो।' : 'This calendar data is calculated using initial calculation.'}
          </span>
        </div>
      )}
    </div>
  );
};

export default CalendarGrid;