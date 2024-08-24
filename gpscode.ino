#define BLYNK_TEMPLATE_ID "TMPL6W6QiPhoB"
#define BLYNK_TEMPLATE_NAME "gps"
#define BLYNK_AUTH_TOKEN "wm9lYKJECPhtQuZMPjQWCZtR8DNHF1aA"

#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// GPS module connected to Serial2 on ESP32
static const int RXPin = 16, TXPin = 17;
static const uint32_t GPSBaud = 9600;

// TinyGPS++ object
TinyGPSPlus gps;

// Blynk virtual pins
#define VIRTUAL_PIN_LAT V1
#define VIRTUAL_PIN_LON V2

// WiFi credentials
char ssid[] = "Shafiqul Haque Residence";
char pass[] = "zigzigzag";

// Create a hardware serial object for the GPS module
HardwareSerial ss(2);

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);
  
  // Start the GPS hardware serial communication
  ss.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);

  // Connect to WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  // Listen to the GPS module
  while (ss.available() > 0) {
    gps.encode(ss.read());
    if (gps.location.isUpdated()) {
      // Get latitude and longitude
      float latitude = gps.location.lat();
      float longitude = gps.location.lng();

      // Print to Serial Monitor
      Serial.print("Latitude: ");
      Serial.println(latitude, 6);
      Serial.print("Longitude: ");
      Serial.println(longitude, 6);

      Serial.print("Speed: ");
      Serial.println(gps.speed.kmph());

      Serial.print("Satellites: ");
      Serial.println(gps.satellites.value());

      // Convert latitude and longitude to strings
      String latitudeString = String(latitude, 6);
      String longitudeString = String(longitude, 6);

      // Send the strings to Blynk virtual pins V1 and V2
      Blynk.virtualWrite(VIRTUAL_PIN_LAT, latitudeString);
      Blynk.virtualWrite(VIRTUAL_PIN_LON, longitudeString);
    }
  }
  
  // Run Blynk
  Blynk.run();
   
}

