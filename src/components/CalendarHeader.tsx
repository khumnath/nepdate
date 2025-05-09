
import React from 'react';
import { ChevronLeftIcon, ChevronRightIcon } from '@heroicons/react/24/solid';
import { nepaliMonthsEn, nepaliMonthsNp, getNepaliDigits } from '../utils/bikramConverter';
import { Button } from './ui/button';

interface CalendarHeaderProps {
  currentMonth: number; // 1-12
  currentYear: number;
  onPrevMonth: () => void;
  onNextMonth: () => void;
  onTodayClick: () => void;
  useNepaliLanguage: boolean;
}

const CalendarHeader: React.FC<CalendarHeaderProps> = ({
  currentMonth,
  currentYear,
  onPrevMonth,
  onNextMonth,
  onTodayClick,
  useNepaliLanguage
}) => {
  const monthName = useNepaliLanguage 
    ? nepaliMonthsNp[currentMonth - 1] 
    : nepaliMonthsEn[currentMonth - 1];
  
  const yearDisplay = useNepaliLanguage 
    ? getNepaliDigits(currentYear) 
    : currentYear;
    
  return (
    <div className="flex justify-between items-center py-4 px-2 mb-2">
      <div className="flex items-center space-x-1">
        <h2 className="text-2xl font-bold text-nepali-dark">
          {monthName} {yearDisplay}
        </h2>
      </div>
      <div className="flex space-x-2">
        <Button 
          onClick={onTodayClick} 
          variant="outline"
          className="text-sm bg-nepali-yellow/10 border-nepali-yellow hover:bg-nepali-yellow hover:text-nepali-dark"
        >
          {useNepaliLanguage ? 'आज' : 'Today'}
        </Button>
        <Button
          onClick={onPrevMonth}
          variant="outline"
          className="p-1 bg-nepali-red/10 border-nepali-red hover:bg-nepali-red hover:text-white"
          size="icon"
        >
          <ChevronLeftIcon className="h-5 w-5" />
        </Button>
        <Button
          onClick={onNextMonth}
          variant="outline"
          className="p-1 bg-nepali-red/10 border-nepali-red hover:bg-nepali-red hover:text-white"
          size="icon"
        >
          <ChevronRightIcon className="h-5 w-5" />
        </Button>
      </div>
    </div>
  );
};

export default CalendarHeader;
