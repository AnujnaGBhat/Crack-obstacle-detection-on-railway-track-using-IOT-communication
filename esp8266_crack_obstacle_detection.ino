/*
 * Crack/Obstacle Detection on Railway Track using IoT Communication
 * Standalone ESP8266-12 firmware
 *
 * Reads an IR sensor (crack detection) and an ultrasonic sensor
 * (obstacle detection), then pushes an alert to ThingSpeak when
 * a fault is detected.
 *
 * Hardware:
 *  - ESP8266-12 module
 *  - IR sensor        -> D2
 *  - HC-SR04 Trig      -> D3
 *  - HC-SR04 Echo      -> D4
 */

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define IR_PIN D2
#define TRIG_PIN D3
#define ECHO_PIN D4

// TODO: replace with your own credentials before uploading.
// Do NOT commit real credentials to a public repository.
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const String apiKey = "YOUR_THINGSPEAK_WRITE_API_KEY";

long duration;
int distance;
bool alertSent = false;

void setup() {
  Serial.begin(115200);
  pinMode(IR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  distance = getDistance();
  int irStatus = digitalRead(IR_PIN);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm, IR: ");
  Serial.println(irStatus == LOW ? "Detected" : "Clear");

  if ((distance < 20 || irStatus == LOW) && !alertSent) {
    Serial.println("Alert: Obstacle or crack detected");
    sendToThingSpeak();
    alertSent = true;
    delay(15000); // respect ThingSpeak's update rate limit
  }

  if (distance >= 20 && irStatus == HIGH) {
    alertSent = false;
  }

  delay(500);
}

int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
  return duration * 0.034 / 2;
}

void sendToThingSpeak() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.thingspeak.com/update?api_key=" + apiKey + "&field1=1";

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.print("ThingSpeak response code: ");
      Serial.println(httpCode);
    } else {
      Serial.print("Failed to send data: ");
      Serial.println(http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}
