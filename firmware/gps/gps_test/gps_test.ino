#include <Adafruit_GPS.h>

// what's the name of the hardware serial port?
#define GPSSerial Serial1
// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);
// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false
uint32_t timer = millis();

const int ledPin = LED_PWR; // pin to use for the LED



void setup()
{
  Serial.begin(115200);

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate

  pinMode(ledPin, OUTPUT);
  
  delay(1000);
  // digitalWrite(ledPin, 1);
  
  // delay(1000);
  // digitalWrite(ledPin, 0);
  
  // delay(1000);
  // digitalWrite(ledPin, 1);
}


void loop() {
  char c = GPS.read();
  
  if (GPS.newNMEAreceived()) {
    // Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }

  if (timer > millis()) timer = millis();

  //GPS to print latitude and longitude
  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 5000) {
    timer = millis(); // reset the timer
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
    }
  }
}

// void setup() {
//   // wait for hardware serial to appear
//   while (!Serial);

//   // make this baud rate fast enough to we aren't waiting on it
//   Serial.begin(115200);

//   // 9600 baud is the default rate for the Ultimate GPS
//   GPSSerial.begin(9600);
//   Serial.write("Start...");
// }


// void loop() {
//   if (Serial.available()) {
//     char c = Serial.read();
//     GPSSerial.write(c);
//     //Serial.write("Writing to GPS\n");
//   }
//   if (GPSSerial.available()) {
//     //Serial.write("GPS serial available...\n");
//     char c = GPSSerial.read();
//     Serial.write(c);
//   }
// }