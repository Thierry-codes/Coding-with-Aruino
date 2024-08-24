#include <WiFiNINA.h>
#include <ArduinoJson.h>

// Your network credentials
char ssid[] = "your-network-SSID";
char pass[] = "your-network-PASSWORD";

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
