import { useState, useEffect } from 'react';
import { BikramDateObj, BikramMonth, getToday, getBikramMonth, getNepaliDigits, containsNepaliDigits, getEnglishDigits } from '../utils/bikramConverter';
import { EventModalData } from '../types/events';
import { CalendarEvent } from '../types/events';
import { loadEventsForYear } from '../utils/events';
import { BS_START_YEAR, BS_END_YEAR } from '../utils/bikram';
import { calculateTithi } from '../utils/tithiCalculation';
import { getAllEventText, getAllEventDetails } from '../utils/events';

export function useCalendarState() {
  const [today, setToday] = useState<BikramDateObj>(getToday());
  const [currentView, setCurrentView] = useState<BikramMonth>(() => getBikramMonth(today.year, today.month));
  const [selectedDate, setSelectedDate] = useState<BikramDateObj | null>(null);
  const [useNepaliLanguage, setUseNepaliLanguage] = useState<boolean>(true);
  const [yearInput, setYearInput] = useState<string>(useNepaliLanguage ? getNepaliDigits(today.year) : today.year.toString());

  interface EventsState {
    bikramFixedEvents: CalendarEvent[];
    gregorianEvents: CalendarEvent[];
    bikramRecurringEvents: CalendarEvent[];
  }
  const [events, setEvents] = useState<EventsState>({
    bikramFixedEvents: [],
    gregorianEvents: [],
    bikramRecurringEvents: [],
  });

  const [eventModalOpen, setEventModalOpen] = useState<boolean>(false);
  const [eventModalData, setEventModalData] = useState<EventModalData | null>(null);
  const [usingApproximation, setUsingApproximation] = useState<boolean>(false);

  useEffect(() => {
    const newMonthData = getBikramMonth(currentView.year, currentView.month);
    setCurrentView(newMonthData);
    setUsingApproximation(currentView.year < BS_START_YEAR || currentView.year > BS_END_YEAR);
    loadEventsForYear(currentView.year).then(loadedEvents => setEvents(loadedEvents));
  }, [currentView.year, currentView.month]);

  const handlePrevMonth = () => {
    setCurrentView(prev => {
      let newMonth = prev.month - 1;
      let newYear = prev.year;
      if (newMonth < 1) {
        newMonth = 12;
        newYear -= 1;
      }
      return getBikramMonth(newYear, newMonth);
    });
  };

  const handleNextMonth = () => {
    setCurrentView(prev => {
      let newMonth = prev.month + 1;
      let newYear = prev.year;
      if (newMonth > 12) {
        newMonth = 1;
        newYear += 1;
      }
      return getBikramMonth(newYear, newMonth);
    });
  };

  const handleTodayClick = () => {
    const todayDate = getToday();
    setCurrentView(getBikramMonth(todayDate.year, todayDate.month));
    setSelectedDate(todayDate);
    setYearInput(useNepaliLanguage ? getNepaliDigits(todayDate.year) : todayDate.year.toString());
  };

  const handleMonthChange = (e: React.ChangeEvent<HTMLSelectElement> | string) => {
    if (typeof e !== 'string' && e.stopPropagation) e.stopPropagation();
    const month = typeof e === 'string' ? parseInt(e) : parseInt(e.target.value);
    setTimeout(() => {
      setCurrentView(prev => getBikramMonth(prev.year, month));
    }, 50);
  };

  const handleYearChange = (value: string) => {
    const year = parseInt(value);
    if (!isNaN(year)) {
      setYearInput(useNepaliLanguage ? getNepaliDigits(year) : year.toString());
      setCurrentView(prev => getBikramMonth(year, prev.month));
    }
  };

  const handleYearInputChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const inputValue = e.target.value;
    setYearInput(inputValue);
    // Only try to update the year if we have a meaningful value
    if (inputValue.trim().length > 0) {
      let year: number;
      if (containsNepaliDigits(inputValue)) {
        year = parseInt(getEnglishDigits(inputValue));
      } else {
        year = parseInt(inputValue);
      }
      // Don't update view yet, wait for submit
    }
  };

  const handleYearSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    let year: number;
    if (containsNepaliDigits(yearInput)) {
      year = parseInt(getEnglishDigits(yearInput));
    } else {
      year = parseInt(yearInput);
    }
    if (!isNaN(year)) {
      setCurrentView(prev => getBikramMonth(year, prev.month));
    } else {
      setYearInput(useNepaliLanguage ? getNepaliDigits(currentView.year) : currentView.year.toString());
    }
  };

  // Updated: Accepts a source argument to control modal opening
  const handleDateSelect = (day: number, source: "calendar" | "converter" = "calendar") => {
    const selectedDateObj: BikramDateObj = {
      year: currentView.year,
      month: currentView.month,
      day: day,
      englishDate: new Date(currentView.englishStartDate)
    };
    selectedDateObj.englishDate.setDate(currentView.englishStartDate.getDate() + (day - 1));
    setSelectedDate(selectedDateObj);

    const englishDate = new Date(currentView.englishStartDate);
    englishDate.setDate(currentView.englishStartDate.getDate() + (day - 1));
    const tithiData = calculateTithi(englishDate);

    const hasEvents = events.bikramFixedEvents.some(event => {
      if (event.date === `${currentView.year}/${currentView.month}/${day}`) return true;
      return false;
    }) || events.bikramRecurringEvents.some(event => {
      if (event.date === `${currentView.month}/${day}`) {
        if (event.startYear && event.endYear) {
          return currentView.year >= event.startYear && currentView.year <= event.endYear;
        }
        return true;
      }
      return false;
    });

    const eventText = hasEvents ? getAllEventText(
      events.bikramFixedEvents,
      events.gregorianEvents,
      events.bikramRecurringEvents,
      currentView.year,
      currentView.month,
      day,
      englishDate.getFullYear(),
      englishDate.getMonth() + 1,
      englishDate.getDate(),
      useNepaliLanguage
    ) : '';

    const eventDetail = hasEvents ? getAllEventDetails(
      events.bikramFixedEvents,
      events.gregorianEvents,
      events.bikramRecurringEvents,
      currentView.year,
      currentView.month,
      day,
      englishDate.getFullYear(),
      englishDate.getMonth() + 1,
      englishDate.getDate()
    ) : '';

    // Only open the modal if source is "calendar"
    if (source === "calendar") {
      setEventModalData({
        day,
        year: currentView.year,
        month: currentView.month,
        tithiName: useNepaliLanguage ? tithiData.tithiName : tithiData.tithiNameEn,
        tithiPaksha: useNepaliLanguage ? tithiData.paksha : tithiData.pakshaEn,
        englishDate,
        eventText,
        eventDetail
      });
      setEventModalOpen(true);
    }
  };

  const toggleLanguage = () => {
    setUseNepaliLanguage(prev => !prev);
    setYearInput(prev => {
      if (containsNepaliDigits(prev)) {
        return getEnglishDigits(prev);
      } else {
        return getNepaliDigits(parseInt(prev));
      }
    });
  };

  // Updated: Pass "converter" as source so modal does not open
  const handleDateNavigate = (year: number, month: number, day: number) => {
    setCurrentView(getBikramMonth(year, month));
    setYearInput(useNepaliLanguage ? getNepaliDigits(year) : year.toString());
    const selectedDateObj: BikramDateObj = {
      year,
      month,
      day,
      englishDate: new Date()
    };
    setSelectedDate(selectedDateObj);
    setTimeout(() => {
      handleDateSelect(day, "converter");
    }, 200);
  };

  return {
    today,
    currentView,
    selectedDate,
    events,
    setCurrentView,
    setSelectedDate,
    handlePrevMonth,
    handleNextMonth,
    handleTodayClick,
    handleMonthChange,
    handleDateSelect,
    handleYearInputChange,
    handleYearSubmit,
    yearInput,
    setYearInput,
    toggleLanguage,
    useNepaliLanguage,
    usingApproximation,
    eventModalOpen,
    setEventModalOpen,
    eventModalData,
    setEventModalData,
    handleDateNavigate,
    handleYearChange,
  };
}