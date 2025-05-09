
import React, { useState, useEffect } from 'react';
import { Dialog, DialogContent, DialogHeader, DialogTitle, DialogDescription, DialogFooter } from "./ui/dialog";
import { Button } from './ui/button';
import { format } from 'date-fns';
import { nepaliMonthsEn, nepaliMonthsNp, getNepaliDigits } from '../utils/bikramConverter';
import { EventModalProps } from '../types/events';

const EventModal: React.FC<EventModalProps> = ({
  isOpen,
  onClose,
  eventData,
  useNepaliLanguage
}) => {
  // Animation state
  const [isAnimating, setIsAnimating] = useState(true);

  // Reset animation state when modal opens
  React.useEffect(() => {
    if (isOpen) {
      setIsAnimating(true);
    }
  }, [isOpen]);

  if (!eventData) return null;

  const bikramDateFormatted = useNepaliLanguage ? 
    `${getNepaliDigits(eventData.day)} ${nepaliMonthsNp[eventData.month - 1]} ${getNepaliDigits(eventData.year)}` : 
    `${eventData.day} ${nepaliMonthsEn[eventData.month - 1]} ${eventData.year}`;
  
  const gregorianDateFormatted = format(eventData.englishDate, 'PPP');
  const weekday = format(eventData.englishDate, 'EEEE');

  const handleClose = () => {
    setIsAnimating(false);
    setTimeout(onClose, 300); // Wait for exit animation
  };

  return (
    <Dialog open={isOpen} onOpenChange={() => handleClose()}>
      <DialogContent className="max-w-md overflow-hidden rounded-xl bg-white dark:bg-slate-900 p-0 font-mukta">
        <div className={`bg-gradient-to-br from-blue-700 to-blue-600 dark:from-blue-900 dark:to-blue-800 p-6 text-white transition-all duration-300 rounded-t-xl ${isAnimating ? 'opacity-100 translate-y-0' : 'opacity-0 translate-y-5'}`}>
          <DialogTitle className="text-center">
            <span className={`inline-block text-2xl font-bold font-laila transition-transform duration-500 ease-spring ${isAnimating ? 'scale-100' : 'scale-90'}`}>
              {useNepaliLanguage ? bikramDateFormatted : bikramDateFormatted}
              <span className="ml-2 text-lg opacity-80">{useNepaliLanguage ? "बि.सं." : "BS"}</span>
            </span>
          </DialogTitle>
          <DialogDescription className="text-center font-medium text-white/80">
            {gregorianDateFormatted} ({weekday})
          </DialogDescription>
        </div>
        
        <div className="space-y-4 p-6">
          <div className={`flex flex-col space-y-4 text-center transition-all duration-300 delay-200 ${isAnimating ? 'opacity-100 translate-y-0' : 'opacity-0 translate-y-5'}`}>
            {/* Tithi information - always shown */}
            <div className="bg-orange-50 dark:bg-orange-950/30 p-3 rounded-lg">
              <div className="text-lg font-semibold text-orange-700 dark:text-orange-400">
                {eventData.tithiPaksha || "N/A"}, {eventData.tithiName || "N/A"}
              </div>
            </div>
            
            {/* Special event information */}
            {eventData.eventText && (
              <div className={`mt-4 pt-4 border-t border-gray-200 dark:border-gray-700 transition-all duration-300 delay-300 ${isAnimating ? 'opacity-100 translate-y-0' : 'opacity-0 translate-y-5'}`}>
                <div className="font-bold text-lg text-gray-800 dark:text-gray-200 mb-2">
                  {useNepaliLanguage ? "विशेष दिन" : "Special Day"}
                </div>
                
                {/* Event name */}
                <div className="text-base font-medium text-red-600 dark:text-red-400 bg-red-50 dark:bg-red-950/30 p-3 rounded-lg">
                  {eventData.eventText}
                </div>
                
                {/* Event details - always shown when available */}
                {eventData.eventDetail && (
                  <div className="text-sm mt-2 text-gray-600 dark:text-gray-400 bg-gray-50 dark:bg-gray-900/50 p-3 rounded-lg">
                    {eventData.eventDetail}
                  </div>
                )}
              </div>
            )}
          </div>
        </div>
        
        <DialogFooter className="p-4 border-t border-gray-200 dark:border-gray-800 bg-gray-50 dark:bg-gray-900 px-0 py-0">
          <div className="w-full transition-transform duration-200 hover:scale-[1.02] active:scale-[0.98]">
            <Button onClick={handleClose} className="w-full bg-blue-600 hover:bg-blue-700 dark:bg-blue-800 dark:hover:bg-blue-700 shadow-md rounded-md font-mukta">
              {useNepaliLanguage ? "बन्द गर्नुहोस्" : "Close"}
            </Button>
          </div>
        </DialogFooter>
      </DialogContent>
    </Dialog>
  );
};

export default EventModal;
