
import React, { useState, useRef, useEffect } from 'react';
import {
  BikramDateObj,
  convertToBikram,
  convertToEnglish,
  getToday,
  nepaliMonthsEn,
  nepaliMonthsNp,
  getNepaliDigits,
  getEnglishDigits,
  containsNepaliDigits,
} from '../utils/bikramConverter';
import { DialogTitle, DialogDescription, DialogClose } from './ui/dialog';
import { Button } from './ui/button';
import { Calendar, X } from 'lucide-react';

interface DateConverterProps {
  useNepaliLanguage: boolean;
  onDateSelect?: (year: number, month: number, day: number) => void;
}

const DateConverter: React.FC<DateConverterProps> = ({ useNepaliLanguage, onDateSelect }) => {
  const [bikramDate, setBikramDate] = useState<BikramDateObj>(getToday());
  const [englishDate, setEnglishDate] = useState<Date>(new Date());
  const handleOpenCalendar = () => {
    if (onDateSelect) {
      onDateSelect(bikramDate.year, bikramDate.month, bikramDate.day);
    }
  };
  
  // Refs for inputs - we'll use these instead of autofocus
  const yearInputRef = useRef<HTMLInputElement>(null);
  const monthSelectRef = useRef<HTMLSelectElement>(null);
  const dayInputRef = useRef<HTMLInputElement>(null);

  // For handling Nepali digit conversion in year input
  const [yearInputText, setYearInputText] = useState<string>(
    useNepaliLanguage ? getNepaliDigits(bikramDate.year) : bikramDate.year.toString()
  );
  
  // Update year input text when language changes
  useEffect(() => {
    setYearInputText(useNepaliLanguage ? getNepaliDigits(bikramDate.year) : bikramDate.year.toString());
  }, [useNepaliLanguage, bikramDate.year]);

  // Handle Bikram date field changes
  const handleBikramDateChange = (field: 'year' | 'month' | 'day', value: number) => {
    setBikramDate(prev => {
      const newDate = { ...prev, [field]: value };
      const convertedDate = convertToEnglish(newDate);
      setEnglishDate(convertedDate);
      
      // Update year input text if the year field is changed
      if (field === 'year') {
        setYearInputText(useNepaliLanguage ? getNepaliDigits(value) : value.toString());
      }
      
      return { ...newDate, englishDate: convertedDate };
    });
  };

  // Handle year input change with Nepali digit conversion
  const handleYearInputChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const inputValue = e.target.value;
    setYearInputText(inputValue);

   // Allow the input to be empty
   if (inputValue.trim() === '') {
    return;
  }
    
    let yearValue: number;
    
    // Convert to number (handling Nepali digits)
    if (containsNepaliDigits(inputValue)) {
      yearValue = parseInt(getEnglishDigits(inputValue)) || bikramDate.year;
    } else {
      yearValue = parseInt(inputValue) || bikramDate.year;
    }
    
    // No year input limit, we'll use fallback calculation for any year
    if (!isNaN(yearValue)) {
      handleBikramDateChange('year', yearValue);
    }
  };

  // Handle English date change
  const handleEnglishDateChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const newDate = new Date(e.target.value);
    if (!isNaN(newDate.getTime())) {
      setEnglishDate(newDate);
      const newBikramDate = convertToBikram(newDate);
      setBikramDate(newBikramDate);
      setYearInputText(useNepaliLanguage ? getNepaliDigits(newBikramDate.year) : newBikramDate.year.toString());
    }
  };

  const formatDay = (day: number) => useNepaliLanguage ? getNepaliDigits(day) : day;

  // Handle day input change with Nepali digit conversion
  const handleDayInputChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const inputValue = e.target.value;
    let dayValue: number;
    
    // Convert to number (handling Nepali digits)
    if (containsNepaliDigits(inputValue)) {
      dayValue = parseInt(getEnglishDigits(inputValue)) || 1;
    } else {
      dayValue = parseInt(inputValue) || 1;
    }
    
    if (!isNaN(dayValue) && dayValue >= 1 && dayValue <= 32) {
      handleBikramDateChange('day', dayValue);
    }
  };

  return (
    <div className="bg-gradient-to-b from-blue-50 to-white font-mukta">
      {/* Header with close button */}
      <div className="bg-gradient-to-r from-blue-700 to-blue-600 px-4 py-3 rounded-t-xl flex justify-between items-center">
        <div>
          <DialogTitle className="text-xl font-semibold text-white font-mukta">
            {useNepaliLanguage ? 'मिति परिवर्तक' : 'Date Converter'}
          </DialogTitle>
          <DialogDescription className="text-sm text-blue-100 font-mukta">
            {useNepaliLanguage ? 'बिक्रम सम्वत र ग्रेगोरियन बीच' : 'Between Bikram Sambat & Gregorian'}
          </DialogDescription>
        </div>
        <DialogClose className="rounded-full h-8 w-8 flex items-center justify-center bg-blue-800 hover:bg-blue-900 text-white transition-colors">
          <X className="h-4 w-4" />
          <span className="sr-only">Close</span>
        </DialogClose>
      </div>

      <div className="p-4">
        <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
          {/* Bikram Date Section */}
          <div className="bg-gradient-to-br from-blue-50 to-blue-100 p-3 rounded-lg border border-blue-200 shadow-inner">
            <h4 className="font-medium mb-2 text-blue-700 text-base font-mukta">
              {useNepaliLanguage ? 'बिक्रम सम्बत' : 'Bikram Sambat (BS)'}
            </h4>
            <div className="grid grid-cols-3 gap-2">
              <div>
                <label className="block text-xs text-gray-700 mb-1 font-mukta">
                  {useNepaliLanguage ? 'वर्ष' : 'Year'}
                </label>
                <input
                  ref={yearInputRef}
                  type="text"
                  value={yearInputText}
                  onChange={handleYearInputChange}
                  className="w-full px-2 py-1 rounded-md border border-blue-300 focus:ring-2 focus:ring-blue-400 text-blue-800 text-sm font-mukta"
                />
              </div>
              <div>
                <label className="block text-xs text-gray-700 mb-1 font-mukta">
                  {useNepaliLanguage ? 'महिना' : 'Month'}
                </label>
                <select
                  ref={monthSelectRef}
                  value={bikramDate.month}
                  onChange={(e) => handleBikramDateChange('month', parseInt(e.target.value))}
                  className="w-full px-2 py-1 rounded-md border border-blue-300 focus:ring-2 focus:ring-blue-400 text-blue-800 text-sm font-mukta"
                >
                  {Array.from({ length: 12 }).map((_, i) => (
                    <option key={i} value={i + 1} className="font-mukta">
                      {useNepaliLanguage ? nepaliMonthsNp[i] : nepaliMonthsEn[i]}
                    </option>
                  ))}
                </select>
              </div>
              <div>
                <label className="block text-xs text-gray-700 mb-1 font-mukta">
                  {useNepaliLanguage ? 'दिन' : 'Day'}
                </label>
                <input
                  ref={dayInputRef}
                  type="text"
                  min="1"
                  max="32"
                  value={useNepaliLanguage ? getNepaliDigits(bikramDate.day) : bikramDate.day}
                  onChange={handleDayInputChange}
                  className="w-full px-2 py-1 rounded-md border border-blue-300 focus:ring-2 focus:ring-blue-400 text-blue-800 text-sm font-mukta"
                />
              </div>
            </div>
          </div>

          {/* Gregorian Date Section */}
          <div className="bg-gradient-to-br from-gray-50 to-gray-100 p-3 rounded-lg border border-gray-200 shadow-inner">
            <h4 className="font-medium mb-2 text-gray-700 text-base font-mukta">
              {useNepaliLanguage ? 'ग्रेगोरियन' : 'Gregorian (AD)'}
            </h4>
            <div>
              <label className="block text-xs text-gray-700 mb-1 font-mukta">
                {useNepaliLanguage ? 'मिति' : 'Date'}
              </label>
              <input
                type="date"
                value={englishDate.toISOString().split('T')[0]}
                onChange={handleEnglishDateChange}
                className="w-full px-2 py-1 rounded-md border border-gray-300 focus:ring-2 focus:ring-blue-400 text-gray-800 text-sm font-mukta"
              />
            </div>
            <p className="mt-2 text-xs text-gray-500 font-mukta">
              {englishDate.toLocaleDateString()}
            </p>
          </div>
        </div>

        {/* Results Section */}
        <div className="mt-4 p-3 text-center bg-gradient-to-r from-blue-100 via-blue-50 to-blue-100 border border-blue-200 rounded-lg">
          <p className="text-blue-800 font-medium text-sm md:text-base font-mukta">
            {useNepaliLanguage ?
              `${getNepaliDigits(bikramDate.day)} ${nepaliMonthsNp[bikramDate.month - 1]} ${getNepaliDigits(bikramDate.year)} बि.सं.` :
              `${bikramDate.day} ${nepaliMonthsEn[bikramDate.month - 1]} ${bikramDate.year} BS`}
            <span className="mx-2">=</span>
            {englishDate.toLocaleDateString()}
          </p>
        </div>

        {/* Navigation Button */}
        {onDateSelect && (
          <div className="mt-4 flex justify-center">
            <Button 
              onClick={handleOpenCalendar} 
              className="bg-blue-600 hover:bg-blue-700 text-white px-4 py-2 rounded-md flex items-center gap-2 transition-colors font-mukta"
            >
              <Calendar className="h-4 w-4" />
              {useNepaliLanguage ? 'क्यालेन्डरमा देखाउनुहोस्' : 'Show in Calendar'}
            </Button>
          </div>
        )}
      </div>
    </div>
  );
};

export default DateConverter;
