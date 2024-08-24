#include <RTClib.h>
#include <Wire.h>

// Define bell pins
const int periodicBell = 8; 
const int activityBell = 9;

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

  if (now.dayOfTheWeek() >= 1 && now.dayOfTheWeek() <= 5) {
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

// Your network credentials
char ssid[] = "your-network-SSID";
char pass[] = "your-network-PASSWORD";
//arduino linking app and the program for controlling remotely
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  
  // Attempt to connect to WiFi network
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");          // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor

        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:application/json");
            client.println();

            // Handle incoming data
            if (client.available()) {
              StaticJsonDocument<512> doc;
              DeserializationError error = deserializeJson(doc, client);
              
              if (!error) {
                const char* wakeUp = doc["wakeUp"];
                const char* breakfast = doc["breakfast"];
                // Parse the rest of the timetable similarly

                // Update the timetable in your code here
                Serial.println("Timetable updated:");
                Serial.println(wakeUp);
                Serial.println(breakfast);
                // Print the rest of the times

                // Respond to the client
                client.println("{\"status\":\"success\"}");
              } else {
                client.println("{\"status\":\"error\"}");
              }
            }

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
