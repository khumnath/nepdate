
import { getNepaliDigits, nepaliMonthsEn, nepaliMonthsNp } from '@/utils/bikramConverter';

// Format date text based on language preference
export const formatDate = (month: number, day: number, year: number, useNepaliLanguage: boolean): string => {
  if (useNepaliLanguage) {
    return `${nepaliMonthsNp[month - 1]} ${getNepaliDigits(day)}, ${getNepaliDigits(year)}`;
  } else {
    return `${nepaliMonthsEn[month - 1]} ${day}, ${year}`;
  }
};

// Format days remaining text
export const formatDaysRemaining = (days: number, useNepaliLanguage: boolean): string => {
  if (days === 0) {
    return useNepaliLanguage ? 'आज' : 'Today';
  } else if (days === 1) {
    return useNepaliLanguage ? 'भोलि' : 'Tomorrow';
  } else {
    return useNepaliLanguage 
      ? `${getNepaliDigits(days)} ${days > 1 ? 'दिन बाँकी' : 'दिन बाँकी'}` 
      : `In ${days} ${days > 1 ? 'days' : 'day'}`;
  }
};

// Get event item left border color
export const getEventItemLeftBorderColor = (index: number, isHoliday: boolean): string => {
  if (isHoliday) return "border-red-500";
  return index === 0 ? "border-l-4 border-l-green-500" : "border-l-4 border-l-blue-500";
};
