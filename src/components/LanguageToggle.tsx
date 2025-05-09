
import React from 'react';
import { Button } from './ui/button';

interface LanguageToggleProps {
  useNepaliLanguage: boolean;
  onToggle: () => void;
}

const LanguageToggle: React.FC<LanguageToggleProps> = ({ useNepaliLanguage, onToggle }) => {
  return (
    <Button 
      onClick={onToggle} 
      variant="outline"
      size="sm"
      className="h-9 bg-white hover:bg-blue-50 text-blue-700 font-semibold rounded-xl font-mukta"
    >
      {useNepaliLanguage ? 'English' : 'नेपाली'}
    </Button>
  );
};

export default LanguageToggle;
