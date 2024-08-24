#include <RTClib.h>
#include <Wire.h>

// Define bell pins
const int periodicBell = 8; // Pin for the periodic bell
const int activityBell = 9; // Pin for the activity bell

RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);
  pinMode(periodicBell, OUTPUT);
  pinMode(activityBell, OUTPUT);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  DateTime now = rtc.now();

  // Monday to Friday schedule
  if (now.dayOfTheWeek() >= 1 && now.dayOfTheWeek() <= 5) {
    // Morning routine
    if (now.hour() == 5 && now.minute() == 30) {
      ringActivityBell();
    } else if (now.hour() == 6 && now.minute() == 0) {
      ringActivityBell();
    } else if (now.hour() == 6 && now.minute() == 30) {
      ringActivityBell();
    } else if (now.hour() == 7 && now.minute() == 10) {
      ringActivityBell();
    } else if (now.hour() == 8 && now.minute() == 15) {
      ringActivityBell();
    }

    // Lesson periods (40 min intervals)
    if (now.hour() == 8 && now.minute() == 15 || 
        now.hour() == 8 && now.minute() == 55 || 
        now.hour() == 9 && now.minute() == 35 || 
        now.hour() == 10 && now.minute() == 15 || 
        now.hour() == 10 && now.minute() == 55 || 
        now.hour() == 11 && now.minute() == 15 || 
        now.hour() == 11 && now.minute() == 55 || 
        now.hour() == 13 && now.minute() == 40 || 
        now.hour() == 14 && now.minute() == 20 || 
        now.hour() == 15 && now.minute() == 0 || 
        now.hour() == 15 && now.minute() == 40 || 
        now.hour() == 16 && now.minute() == 20) {
      ringPeriodicBell();
    }

    // Other activities
    if (now.hour() == 10 && now.minute() == 55) {
      ringActivityBell();
    } else if (now.hour() == 12 && now.minute() == 35) {
      ringActivityBell();
    } else if (now.hour() == 16 && now.minute() == 20) {
      ringActivityBell();
    } else if (now.hour() == 17 && now.minute() == 30) {
      ringActivityBell();
    } else if (now.hour() == 18 && now.minute() == 0) {
      ringActivityBell();
    } else if (now.hour() == 19 && now.minute() == 30) {
      ringActivityBell();
    } else if (now.hour() == 20 && now.minute() == 30) {
      ringActivityBell();
    } else if (now.hour() == 21 && now.minute() == 30) {
      ringActivityBell();
    }
  }

  // Wednesday & Weekend schedule (suggested)
  if (now.dayOfTheWeek() == 0 || now.dayOfTheWeek() == 3 || now.dayOfTheWeek() == 6) {
    if (now.hour() == 6 && now.minute() == 30) {
      ringActivityBell();
    } else if (now.hour() == 8 && now.minute() == 0) {
      ringActivityBell();
    } else if (now.hour() == 12 && now.minute() == 0) {
      ringActivityBell();
    } else if (now.hour() == 16 && now.minute() == 0) {
      ringActivityBell();
    } else if (now.hour() == 20 && now.minute() == 0) {
      ringActivityBell();
    }
  }

  delay(1000);
}

void ringPeriodicBell() {
  digitalWrite(periodicBell, HIGH);
  delay(8000); // Bell rings for 8 seconds
  digitalWrite(periodicBell, LOW);
}

void ringActivityBell() {
  digitalWrite(activityBell, HIGH);
  delay(8000); // Bell rings for 8 seconds
  digitalWrite(activityBell, LOW);
}
