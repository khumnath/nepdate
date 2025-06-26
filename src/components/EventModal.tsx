import React, { useState, useEffect } from 'react';
import { Dialog, DialogContent, DialogHeader, DialogTitle, DialogDescription, DialogFooter } from "./ui/dialog";
import { Button } from './ui/button';
import { format } from 'date-fns';
import { nepaliMonthsEn, nepaliMonthsNp, getNepaliDigits } from '../utils/bikramConverter';
import { EventModalProps } from '../types/events';
import { calculatePanchanga,
  tithiNp,
  nakshatraNp,
  rashiNp,
  karanNp,
  yogaNp,
  tithi,
  nakshatra,
  rashi,
  karan,
  yoga
} from '../utils/tithiCalculation';

const EventModal: React.FC<EventModalProps> = ({
  isOpen,
  onClose,
  eventData,
  useNepaliLanguage
}) => {
  const [isAnimating, setIsAnimating] = useState(false);
  const [showDebug, setShowDebug] = useState(false);

  useEffect(() => {
    if (isOpen) {
      setIsAnimating(true);
    }
  }, [isOpen]);

  if (!eventData) return null;

  const panchanga = calculatePanchanga(eventData.englishDate);

  const getLabel = (en: string, np: string) => useNepaliLanguage ? np : en;

  const bikramDateFormatted = useNepaliLanguage ?
    `${getNepaliDigits(eventData.day)} ${nepaliMonthsNp[eventData.month - 1]} ${getNepaliDigits(eventData.year)}` :
    `${eventData.day} ${nepaliMonthsEn[eventData.month - 1]} ${eventData.year}`;

  const gregorianDateFormatted = format(eventData.englishDate, 'PPP');
  const weekday = format(eventData.englishDate, 'EEEE');

  const handleClose = () => {
    setIsAnimating(false);
    setTimeout(onClose, 300); // Wait for exit animation
  };

  const getTranslated = (arr: string[], index: number, fallback: string) => {
    return (index >= 0 && index < arr.length) ? arr[index] : fallback;
  };

  const formatAMPMFromString = (time: string) => {
    const [h, m] = time.split(':').map(Number);
    if (isNaN(h) || isNaN(m)) return time;
    const hour = h % 12 || 12;
    const minutes = m.toString().padStart(2, '0');
    if (useNepaliLanguage) {
      const nepDigits = (n: number) => n.toString().replace(/\d/g, d => '०१२३४५६७८९'[+d]);
      const ampm = h < 12 ? 'बिहान' : 'बेलुका';
      return `${nepDigits(hour)}:${nepDigits(parseInt(minutes, 10))} ${ampm}`;
    } else {
      const ampm = h < 12 ? 'AM' : 'PM';
      return `${hour}:${minutes} ${ampm}`;
    }
  };

  return (
    <Dialog open={isOpen} onOpenChange={handleClose}>
      <DialogContent className={`max-w-md w-full m-4 p-0 font-mukta border-none rounded-2xl shadow-2xl bg-slate-50 dark:bg-slate-900 text-slate-800 dark:text-slate-200 transition-all duration-300 ${isAnimating ? 'opacity-100 scale-100' : 'opacity-0 scale-95'}`}>
        {/* Header Section */}
        <div className={`bg-gradient-to-br from-indigo-500 to-blue-500 dark:from-indigo-800 dark:to-blue-800 p-6 text-white rounded-t-2xl`}>
          <DialogTitle className="text-center">
            <span className={`inline-block text-3xl font-bold font-laila transition-transform duration-500 ease-in-out ${isAnimating ? 'scale-100' : 'scale-90'}`}>
              {bikramDateFormatted}
              <span className="ml-2 text-xl opacity-80">{useNepaliLanguage ? "बि.सं." : "BS"}</span>
            </span>
          </DialogTitle>
          <DialogDescription className="text-center text-sm font-medium text-white/80 mt-1">
            {gregorianDateFormatted} ({weekday})
          </DialogDescription>
        </div>

        <div className="p-6 space-y-5">
          {/* Tithi Block */}
          <div className={`text-center bg-white dark:bg-slate-800 p-4 rounded-xl shadow-md transition-opacity duration-300 delay-100 ${isAnimating ? 'opacity-100' : 'opacity-0'}`}>
            <p className="text-xl font-semibold text-orange-600 dark:text-orange-400">
              {panchanga.dtithi !== undefined
                ? (useNepaliLanguage
                    ? `${tithiNp[panchanga.tithiIndex]} (${panchanga.dpaksha === 'Shuklapaksha' ? 'शुक्ल पक्ष' : 'कृष्ण पक्ष'})`
                    : `${tithi[panchanga.tithiIndex] || panchanga.dtithi} (${panchanga.dpaksha})`)
                : (panchanga.error || 'N/A')}
            </p>
          </div>

          {/* Panchanga Details Block */}
          <div className={`bg-white dark:bg-slate-800 p-4 rounded-xl shadow-md transition-opacity duration-300 delay-200 text-sm ${isAnimating ? 'opacity-100' : 'opacity-0'}`}>
            <div className="grid grid-cols-2 gap-x-4 gap-y-2 text-slate-600 dark:text-slate-300">
              <div className="font-semibold">{getLabel('Tithi Start', 'तिथि सुरु')}:</div>       <div>{panchanga.tithiStart}</div>
              <div className="font-semibold">{getLabel('Tithi End', 'तिथि अन्त्य')}:</div>         <div>{panchanga.tithiEnd}</div>
              <div className="font-semibold">{getLabel('Nakshatra', 'नक्षत्र')}:</div>        <div>{useNepaliLanguage ? getTranslated(nakshatraNp, nakshatra.indexOf(panchanga.dnakshatra), panchanga.dnakshatra) : panchanga.dnakshatra}</div>
              <div className="font-semibold">{getLabel('Rashi', 'राशि')}:</div>            <div>{useNepaliLanguage ? getTranslated(rashiNp, rashi.indexOf(panchanga.drashi), panchanga.drashi) : panchanga.drashi}</div>
              <div className="font-semibold">{getLabel('Karan', 'करण')}:</div>            <div>{useNepaliLanguage ? getTranslated(karanNp, karan.indexOf(panchanga.dkarana), panchanga.dkarana) : panchanga.dkarana}</div>
              <div className="font-semibold">{getLabel('Yoga', 'योग')}:</div>             <div>{useNepaliLanguage ? getTranslated(yogaNp, yoga.indexOf(panchanga.dyoga), panchanga.dyoga) : panchanga.dyoga}</div>
              <div className="font-semibold">{getLabel('Sunrise', 'सूर्योदय')}:</div>        <div>{formatAMPMFromString(panchanga.sunrise)}</div>
              <div className="font-semibold">{getLabel('Sunset', 'सूर्यास्त')}:</div>         <div>{formatAMPMFromString(panchanga.sunset)}</div>
            </div>
          </div>

          {/* Special Event Block */}
          {eventData.eventText && (
            <div className={`pt-5 mt-5 border-t border-slate-200 dark:border-slate-700 transition-opacity duration-300 delay-300 ${isAnimating ? 'opacity-100' : 'opacity-0'}`}>
              <h3 className="font-bold text-lg text-slate-800 dark:text-slate-100 mb-3 text-center">
                {getLabel("Special Day", "विशेष दिन")}
              </h3>
              <div className="bg-red-50 dark:bg-red-900/40 p-3 rounded-lg text-center">
                <p className="font-semibold text-red-600 dark:text-red-400">{eventData.eventText}</p>
              </div>
              {eventData.eventDetail && (
                <div className="text-sm mt-2 text-slate-600 dark:text-slate-400 bg-slate-100 dark:bg-slate-800 p-3 rounded-lg">
                  {eventData.eventDetail}
                </div>
              )}
            </div>
          )}

          {/* Debug Section */}
          <div className="pt-3 text-center">
             <Button
                variant="ghost"
                size="sm"
                className="text-xs text-slate-500 dark:text-slate-400 hover:bg-slate-200 dark:hover:bg-slate-700"
                onClick={() => setShowDebug(v => !v)}
              >
                {showDebug ? getLabel('Hide Debug', 'डिबग लुकाउनुहोस्') : getLabel('Show Debug', 'डिबग देखाउनुहोस्')}
              </Button>
              {showDebug && panchanga.debug && (
                <pre className="mt-2 p-3 bg-slate-100 dark:bg-black/30 text-xs text-left rounded-md max-h-48 overflow-auto whitespace-pre-wrap border border-slate-200 dark:border-slate-700 font-mono">
                  {panchanga.debug}
                </pre>
              )}
          </div>
        </div>

        {/* Footer and Close Button */}
        <DialogFooter className="p-3 bg-slate-100/50 dark:bg-slate-800/50">
          <Button
            onClick={handleClose}
            className="w-full bg-indigo-600 hover:bg-indigo-700 dark:bg-indigo-700 dark:hover:bg-indigo-600 text-white font-bold py-3 text-base rounded-lg shadow-lg hover:shadow-indigo-500/50 dark:hover:shadow-indigo-800/50 transition-all duration-300 ease-in-out transform hover:-translate-y-px active:scale-[0.98]">
            {getLabel("Close", "बन्द गर्नुहोस्")}
          </Button>
        </DialogFooter>
      </DialogContent>
    </Dialog>
  );
};

export default EventModal;