/*
 * Crack/Obstacle Detection on Railway Track using IoT Communication
 * Arduino Uno firmware (drives sensors + GPS, talks to ESP-01 over AT commands)
 *
 * Hardware:
 *  - Arduino Uno
 *  - HC-SR04 Trig  -> D4
 *  - HC-SR04 Echo  -> D5
 *  - IR sensor      -> A0
 *  - Buzzer         -> D6
 *  - ESP-01 (Wi-Fi) -> SoftwareSerial on pins 2 (RX), 3 (TX)
 *  - NEO-6M GPS     -> SoftwareSerial on pins 7 (RX), 8 (TX)
 */

#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define TRIG_PIN 4
#define ECHO_PIN 5
#define IR_SENSOR A0
#define BUZZER 6

SoftwareSerial espSerial(2, 3); // (RX, TX) for ESP-01
SoftwareSerial gpsSerial(7, 8); // (RX, TX) for GPS
TinyGPSPlus gps;

// TODO: replace with your own credentials before uploading.
// Do NOT commit real credentials to a public repository.
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* apiKey = "YOUR_THINGSPEAK_WRITE_API_KEY";

void setup() {
  Serial.begin(115200);
  espSerial.begin(9600);
  gpsSerial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(IR_SENSOR, INPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.println("System Initializing...");
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  connectWiFi();
}

void loop() {
  // --- Obstacle Detection ---
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;

  // --- Crack Detection ---
  int crack = analogRead(IR_SENSOR);

  bool obstacleDetected = (distance < 60);
  bool crackDetected = (crack > 100);

  Serial.print("Distance: "); Serial.print(distance); Serial.println(" cm");
  Serial.print("IR Sensor Value: "); Serial.println(crack);

  if (obstacleDetected || crackDetected) {
    digitalWrite(BUZZER, HIGH);
    Serial.println("Warning: Issue Detected!");

    // Get GPS Location
    float latitude = 0.0, longitude = 0.0;
    while (gpsSerial.available()) {
      gps.encode(gpsSerial.read());
      if (gps.location.isUpdated()) {
        latitude = gps.location.lat();
        longitude = gps.location.lng();

        Serial.print("GPS Location - Lat: ");
        Serial.print(latitude, 6);
        Serial.print(", Lon: ");
        Serial.println(longitude, 6);
      }
    }

    // Send data to ThingSpeak via Wi-Fi
    if (WiFiConnected()) {
      Serial.println("Sending data via Wi-Fi...");
      sendDataWiFi(latitude, longitude, obstacleDetected, crackDetected);
    } else {
      Serial.println("Wi-Fi not connected. Data not sent.");
    }
  } else {
    digitalWrite(BUZZER, LOW);
  }

  Serial.println("--------------------------------------------------");
  delay(5000);
}

void connectWiFi() {
  espSerial.println("AT+CWMODE=1");
  delay(2000);
  espSerial.print("AT+CWJAP=\"");
  espSerial.print(ssid);
  espSerial.print("\",\"");
  espSerial.print(password);
  espSerial.println("\"");
  delay(5000);
}

bool WiFiConnected() {
  espSerial.println("AT+CWJAP?");
  delay(2000);
  return espSerial.find("OK");
}

void sendDataWiFi(float lat, float lon, bool obstacle, bool crack) {
  String url = "GET /update?api_key=" + String(apiKey) +
               "&field1=" + String(lat, 6) +
               "&field2=" + String(lon, 6) +
               "&field3=" + String(obstacle) +
               "&field4=" + String(crack);

  espSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80");
  delay(2000);
  espSerial.print("AT+CIPSEND=");
  espSerial.println(url.length() + 2);
  delay(1000);
  espSerial.println(url);
  delay(2000);
  espSerial.println("AT+CIPCLOSE");
  Serial.println("Data sent to ThingSpeak!");
}
