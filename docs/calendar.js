        let currentYear;
        let currentMonth;
        let currentDay;

        function showCurrentMonth() {
            const today = new Date();
            const bikram = new Bikram();
            bikram.fromGregorian(today.getFullYear(), today.getMonth() + 1, today.getDate());
            
            currentYear = bikram.getYear();
            currentMonth = bikram.getMonth();
            currentDay = bikram.getDay();

            document.getElementById('yearInput').value = currentYear;
            document.getElementById('monthSelector').value = currentMonth;

            generateCalendar(currentYear, currentMonth);
        }

        function getfirstWeekday(year, month) {
            const bikram = new Bikram();
            const firstGregorianDate = bikram.toGregorian(year, month, 1);
            const timeinfo = new Date(firstGregorianDate.year, firstGregorianDate.month - 1, firstGregorianDate.day);
            const weekdayIndex = timeinfo.getDay();
            return weekdayIndex;
        }

        function convertToDevanagari(number) {
    const devanagariDigits = ['०', '१', '२', '३', '४', '५', '६', '७', '८', '९'];
    return number.toString().split('').map(digit => devanagariDigits[parseInt(digit, 10)]).join('');
}

function generateCalendar(year, month) {
    const calendarDiv = document.getElementById('calendar');
    calendarDiv.innerHTML = '';

    const daysInMonth = new Bikram().daysInMonth(year, month);
    const bikram = new Bikram();
    
    const today = new Date();
    const todayBikram = new Bikram();
    todayBikram.fromGregorian(today.getFullYear(), today.getMonth() + 1, today.getDate());
    const todayBikramYear = todayBikram.getYear();
    const todayBikramMonth = todayBikram.getMonth();
    const todayBikramDay = todayBikram.getDay();

    const defaultLanguage = "nepali";
    const monthName = getMonthNameWithDefaultLanguage(month, defaultLanguage);
    const gregorianStart = getGregorianDate(year, month, 1);
    const gregorianEnd = getLastDayOfGregorian(year, month);

    const gregorianMonthDisplay = formatGregorianMonthDisplay(gregorianStart, gregorianEnd);

    let calendarHTML = `<h2>${monthName} ${convertToDevanagari(year)} <span class="gregorian-month-display">${gregorianMonthDisplay}</span></h2><table><tr>`;
    
    const weekdays = ['आइतवार', 'सोमवार', 'मङ्गलवार', 'बुधवार', 'बिहीवार', 'शुक्रवार', 'शनिवार'];  // Nepali weekdays
    
    weekdays.forEach(day => {
        calendarHTML += `<th>${day}</th>`;
    });
    calendarHTML += '</tr><tr>';

    const firstDayOfMonth = getfirstWeekday(year, month);

    for (let i = 0; i < firstDayOfMonth; i++) {
        calendarHTML += '<td class="empty"></td>';
    }

    for (let day = 1; day <= daysInMonth; day++) {
        const isToday = (day === todayBikramDay && month === todayBikramMonth && year === todayBikramYear);
        const className = isToday ? 'today' : '';
        
        // Highlight Saturdays (index 6)
        const saturdayClass = (firstDayOfMonth + day - 1) % 7 === 6 ? 'saturday' : '';
        const finalClass = className ? ` ${className}` : '';
        
        // Calculate Tithi
        const gregorianDate = bikram.toGregorian(year, month, day);
        const tithi = calculateTithi(gregorianDate.year, gregorianDate.month, gregorianDate.day);
        
        let tithiImage = '';
        if (tithi.tithi === 'पूर्णिमा') {
            tithiImage = '<img src="purnima.png" alt="Purnima" style="width:30px;height:30px;">';
        } else if (tithi.tithi === 'अमावास्या') {
            tithiImage = '<img src="amawasya.png" alt="Amawasya" style="width:30px;height:30px;">';
        }

        // Check for events
        let eventText = '';
        let eventDetail = '';
        const hasEvent = checkEvent(bikramBasedEvents, year, month, day, 'bikram') ||
                         checkEvent(gregorianBasedEvents, gregorianDate.year, gregorianDate.month, gregorianDate.day, 'gregorian') ||
                         checkEvent(fixedDateEvents, year, month, day, 'fixed');

        // If there is an event, collect the event text and details
        if (hasEvent) {
            eventText = getEventText(bikramBasedEvents, year, month, day, 'bikram') ||
                        getEventText(gregorianBasedEvents, gregorianDate.year, gregorianDate.month, gregorianDate.day, 'gregorian') ||
                        getEventText(fixedDateEvents, year, month, day, 'fixed');

            eventDetail = getEventDetail(bikramBasedEvents, year, month, day, 'bikram') ||
                         getEventDetail(gregorianBasedEvents, gregorianDate.year, gregorianDate.month, gregorianDate.day, 'gregorian') ||
                         getEventDetail(fixedDateEvents, year, month, day, 'fixed');
        }

        // Add the HTML for the calendar cell
        calendarHTML += `<td class="${finalClass} ${saturdayClass}" onclick="showEventDetails('${day}', '${tithi.tithi.replace(/'/g, "\\'")}', '${tithi.paksha.replace(/'/g, "\\'")}', '${gregorianDate.day}', '${bikram.toString()}', '${eventText.replace(/'/g, "\\'")}', '${eventDetail.replace(/'/g, "\\'")}')">
            <div class="day-content">
                <div>${convertToDevanagari(day)}</div>
                <div class="tithi">
                    ${hasEvent ? '' : tithi.tithi}
                    ${tithiImage}
                </div>
                ${hasEvent ? `<div class="event">${eventText}</div>` : ''}
                <div class="gregorian">${gregorianDate.day}</div>
            </div>
        </td>`;

        if ((firstDayOfMonth + day) % 7 === 0 && day !== daysInMonth) {
            calendarHTML += '</tr><tr>';
        }
    }

    const remainingCells = (firstDayOfMonth + daysInMonth) % 7;
    if (remainingCells !== 0) {
        for (let i = 0; i < 7 - remainingCells; i++) {
            calendarHTML += '<td class="empty"></td>';
        }
    }

    calendarHTML += '</tr></table>';
    calendarDiv.innerHTML = calendarHTML;
}





function checkEvent(events, year, month, day, dateType) {
    let hasEvent = false;
    events.forEach(event => {
        if ((event.startYear && year < event.startYear) || (event.endYear && year > event.endYear)) {
            return; // Skip this event if the current year is before the startYear or after the endYear
        }

        if (event.showOnDay === false) {
            return; // Skip this event if it should not be shown on the day cell
        }

        if (dateType === 'bikram') {
            const [eventYear, eventMonth, eventDay] = event.date.split('/').map(Number);
            if (year === eventYear && month === eventMonth && day === eventDay) {
                hasEvent = true;
            }
        } else if (dateType === 'gregorian' || dateType === 'fixed') {
            const [eventMonth, eventDay] = event.date.split('/').map(Number);
            if (month === eventMonth && day === eventDay) {
                hasEvent = true;
            }
        }
    });
    return hasEvent;
}


function getEventText(events, year, month, day, dateType) {
    let eventText = '';
    events.forEach(event => {
        if ((event.startYear && year < event.startYear) || (event.endYear && year > event.endYear)) {
            return; // Skip this event if the current year is before the startYear or after the endYear
        }

        if (event.showOnDay === false) {
            return; // Skip this event if it should not be shown on the day cell
        }

        if (dateType === 'bikram') {
            const [eventYear, eventMonth, eventDay] = event.date.split('/').map(Number);
            if (year === eventYear && month === eventMonth && day === eventDay) {
                eventText += `<div>${event.event}</div>`;
            }
        } else if (dateType === 'gregorian' || dateType === 'fixed') {
            const [eventMonth, eventDay] = event.date.split('/').map(Number);
            if (month === eventMonth && day === eventDay) {
                eventText += `<div>${event.event}</div>`;
            }
        }
    });
    return eventText;
}


function getEventDetail(events, year, month, day, dateType) {
    let eventDetail = '';
    events.forEach(event => {
        if ((event.startYear && year < event.startYear) || (event.endYear && year > event.endYear)) {
            return; // Skip this event if the current year is before the startYear or after the endYear
        }

        if (dateType === 'bikram') {
            const [eventYear, eventMonth, eventDay] = event.date.split('/').map(Number);
            if (year === eventYear && month === eventMonth && day === eventDay) {
                eventDetail += `<div>${event.detail}</div>`;
            }
        } else if (dateType === 'gregorian' || dateType === 'fixed') {
            const [eventMonth, eventDay] = event.date.split('/').map(Number);
            if (month === eventMonth && day === eventDay) {
                eventDetail += `<div>${event.detail}</div>`;
            }
        }
    });
    return eventDetail;
}


function showEventDetails(day, tithi, paksha, gregorianDate, bikramDate, eventText, eventDetail, year, month) {
    // Get the modal element
    const modal = document.getElementById('tithiModal');
    const tithiInfo = document.getElementById('tithiInfo');
    
    
    // Construct the content for the modal
    let content = `
        <p> ${paksha} ${tithi}</p>
    `;

    // If there are events, append event details to the modal content
    if (eventText && eventDetail) {
        content += `
            <p><strong>${eventText}</strong></p>
            <p> ${eventDetail}</p>
        `;
    }

    // Set the modal content
    tithiInfo.innerHTML = content;

    // Display the modal
    modal.style.display = "block";
}

function closeModal() {
    const tithiModal = document.getElementById('tithiModal');
    tithiModal.style.display = 'none';
}

window.onclick = function(event) {
    const modal = document.getElementById('tithiModal');
    if (event.target == modal) {
        modal.style.display = 'none';
    }
}
function closeModal() {
    document.getElementById('tithiModal').style.display = 'none';
}

window.onclick = function(event) {
    const modal = document.getElementById('tithiModal');
    if (event.target == modal) {
        modal.style.display = 'none';
    }
}


        function getMonthNameWithDefaultLanguage(month, language) {
            const bikram = new Bikram();
            const originalGetMonthName = bikram.getMonthName.bind(bikram);
            bikram.getMonthName = function(month) {
                const nepaliMonths = ["बैसाख", "जेष्ठ", "आषाढ", "श्रावण", "भाद्र", "आश्विन", "कार्तिक", "मंसिर", "पौष", "माघ", "फागुन", "चैत"];
                return nepaliMonths[month - 1];
            }

            const monthName = bikram.getMonthName(month);

            return monthName;
        }

        function previousYear() {
            currentYear--;
            document.getElementById('yearInput').value = currentYear;
            generateCalendar(currentYear, currentMonth);
        }

        function nextYear() {
            currentYear++;
            document.getElementById('yearInput').value = currentYear;
            generateCalendar(currentYear, currentMonth);
        }

        function changeMonth() {
            currentMonth = parseInt(document.getElementById('monthSelector').value);
            generateCalendar(currentYear, currentMonth);
        }

        function changeYear() {
            currentYear = parseInt(document.getElementById('yearInput').value);
            generateCalendar(currentYear, currentMonth);
        }

        function goToToday() {
            const today = new Date();
            const bikram = new Bikram();
            bikram.fromGregorian(today.getFullYear(), today.getMonth() + 1, today.getDate());
            currentYear = bikram.getYear();
            currentMonth = bikram.getMonth();
            document.getElementById('yearInput').value = currentYear;
            document.getElementById('monthSelector').value = currentMonth;
            generateCalendar(currentYear, currentMonth);
        }

        function getGregorianDate(year, month, day) {
    const bikram = new Bikram();
    return bikram.toGregorian(year, month, day);
}

function getLastDayOfGregorian(year, month) {
    const bikram = new Bikram();
    const nextMonthFirstDay = bikram.toGregorian(year, month + 1, 1);
    const lastDayOfMonth = new Date(nextMonthFirstDay.year, nextMonthFirstDay.month - 1, nextMonthFirstDay.day - 1);
    return {
        day: lastDayOfMonth.getDate(),
        month: lastDayOfMonth.getMonth() + 1, // months are 0-indexed in JavaScript Date
        year: lastDayOfMonth.getFullYear()
    };
}

function getGregorianMonthName(month) {
    const gregorianMonths = ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'];
    return gregorianMonths[month - 1];
}

function formatGregorianMonthDisplay(gregorianStart, gregorianEnd) {
    if (gregorianStart.year === gregorianEnd.year) {
        if (gregorianStart.month === gregorianEnd.month) {
            return `${gregorianStart.year} ${getGregorianMonthName(gregorianStart.month)}`;
        } else {
            return `${gregorianStart.year} ${getGregorianMonthName(gregorianStart.month)}/${getGregorianMonthName(gregorianEnd.month)}`;
        }
    } else {
        return `${gregorianStart.year}/${gregorianEnd.year} ${getGregorianMonthName(gregorianStart.month)}/${getGregorianMonthName(gregorianEnd.month)}`;
    }
}


        window.onload = showCurrentMonth;