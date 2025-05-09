
export interface CalendarEvent {
  date: string; // Format: "YYYY/MM/DD" for fixed events or "MM/DD" for recurring
  event: string; // Event name in Nepali
  eventEn?: string; // Event name in English
  detail: string; // Event detail in Nepali
  detailEn?: string; // Event detail in English
  startYear?: number;
  endYear?: number;
  showOnDay?: boolean;
  type?: 'bikram' | 'gregorian' | 'recurring';
  isHoliday?: boolean;
}

export interface EventModalProps {
  isOpen: boolean;
  onClose: () => void;
  eventData: {
    day: number;
    year: number;
    month: number;
    tithiName: string;
    tithiPaksha: string;
    englishDate: Date;
    eventText: string;
    eventDetail: string;
  } | null;
  
  useNepaliLanguage: boolean;
}

export interface EventModalData {
  year: number;
  month: number;
  day: number;
  englishDate: Date;
  tithiName: string;
  tithiPaksha: string;
  eventText: string;
  eventDetail: string;
}

export interface EventData {
  day: number;
  tithiName: string;
  tithiPaksha: string;
  englishDate: Date;
  eventText: string;
  eventDetail: string;
}
