
import React from 'react';

interface EventsHeaderProps {
  headerBgColor: string;
  headerTextColor: string;
  useNepaliLanguage: boolean;
}

const EventsHeader: React.FC<EventsHeaderProps> = ({
  headerBgColor,
  headerTextColor,
  useNepaliLanguage,
}) => {
  return (
    <h3 className={`py-2 px-4 ${headerBgColor} ${headerTextColor} font-bold flex justify-between items-center font-mukta rounded-t-lg`}>
      <span>
        {useNepaliLanguage ? 'आगामी कार्यक्रमहरू' : 'Upcoming Events'}
      </span>
    </h3>
  );
};

export default EventsHeader;
