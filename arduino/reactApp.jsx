import React, { useState } from 'react';
import './App.css';

function App() {
  const [dayType, setDayType] = useState('weekday'); // Manage whether it's a weekday or weekend
  const [weekdayTimetable, setWeekdayTimetable] = useState({
    wakeUp: "05:30",
    breakfast: "06:00",
    prep: "06:30",
    mass: "07:10",
    lessonsStart: "08:15",
    breakTime: "10:55",
    lessonsResume: "11:15",
    lunch: "12:35",
    sports: "16:20",
    bathing: "17:30",
    supper: "19:30",
    bedTime: "21:30"
  });

  const [weekendTimetable, setWeekendTimetable] = useState({
    wakeUp: "07:00",
    breakfast: "07:30",
    freeTime: "08:00",
    mass: "09:00",
    lunch: "12:00",
    afternoonActivity: "14:00",
    sports: "16:00",
    bathing: "17:30",
    supper: "19:00",
    freeTimeEvening: "20:00",
    bedTime: "22:00"
  });

  const handleDayTypeChange = (e) => {
    setDayType(e.target.value);
  };

  const handleChange = (e) => {
    const { name, value } = e.target;
    if (dayType === 'weekday') {
      setWeekdayTimetable({ ...weekdayTimetable, [name]: value });
    } else {
      setWeekendTimetable({ ...weekendTimetable, [name]: value });
    }
  };

  const handleSubmit = (e) => {
    e.preventDefault();
    const timetableToSend = dayType === 'weekday' ? weekdayTimetable : weekendTimetable;

    // Assuming Arduino is hosted at a specific IP, replace `your-arduino-ip` with your Arduino's IP address
    fetch(`http://your-arduino-ip/update-timetable?dayType=${dayType}`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify(timetableToSend)
    })
    .then(response => response.json())
    .then(data => {
      alert(`${dayType.charAt(0).toUpperCase() + dayType.slice(1)} timetable updated successfully!`);
    })
    .catch(error => {
      console.error('Error updating timetable:', error);
      alert('Failed to update timetable.');
    });
  };

  const timetable = dayType === 'weekday' ? weekdayTimetable : weekendTimetable;

  return (
    <div className="App">
      <h1>School Bell Timetable</h1>
      <div>
        <label>
          <input
            type="radio"
            value="weekday"
            checked={dayType === 'weekday'}
            onChange={handleDayTypeChange}
          />
          Weekday
        </label>
        <label>
          <input
            type="radio"
            value="weekend"
            checked={dayType === 'weekend'}
            onChange={handleDayTypeChange}
          />
          Weekend
        </label>
      </div>
      <form onSubmit={handleSubmit}>
        {Object.keys(timetable).map((key) => (
          <div key={key}>
            <label>
              {key.replace(/([A-Z])/g, ' $1').charAt(0).toUpperCase() + key.replace(/([A-Z])/g, ' $1').slice(1)}:
              <input
                type="time"
                name={key}
                value={timetable[key]}
                onChange={handleChange}
              />
            </label>
          </div>
        ))}
        <button type="submit">Update Timetable</button>
      </form>
    </div>
  );
}

export default App;
