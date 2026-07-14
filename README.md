# Crack-obstacle-detection-on-railway-track-using-IOT-communication

An IoT-enabled railway safety system that continuously monitors railway tracks for **cracks** and **obstacles** using sensors and sends **real-time alerts** to the cloud. The project aims to improve railway safety by enabling early detection of hazards and reducing the need for manual inspection.

##  Overview

Railway accidents caused by damaged tracks and unexpected obstacles can lead to severe loss of life and property. This project uses **Arduino Uno**, **ESP8266 Wi-Fi module**, **IR Sensor**, **Ultrasonic Sensor**, and **GPS (NEO-6M)** to detect cracks and obstacles, determine their location, and upload the information to **ThingSpeak** for remote monitoring.

##  Features

- Real-time railway track monitoring
- Crack detection using IR sensor
- Obstacle detection using Ultrasonic sensor
- GPS-based fault location tracking
- Wi-Fi communication using ESP8266
- Cloud monitoring with ThingSpeak
- Instant alert generation for detected faults
- Low-cost and scalable IoT solution

##  Hardware Used

- Arduino Uno
- ESP8266-12 Wi-Fi Module
- HC-SR04 Ultrasonic Sensor
- IR Sensor Module
- NEO-6M GPS Module
- Buzzer
- Connecting wires & Breadboard
- Power Supply

##  Software Used

- Arduino IDE
- ThingSpeak Cloud Platform

##  Working Principle

1. The system initializes all sensors and connects to Wi-Fi.
2. The IR sensor continuously monitors the railway track for cracks.
3. The Ultrasonic sensor detects obstacles ahead of the track.
4. If a crack or obstacle is detected:
   - The buzzer is activated.
   - GPS coordinates are captured.
   - Sensor data is transmitted to ThingSpeak using the ESP8266.
5. Railway authorities can remotely monitor the status through the cloud dashboard.

##  IoT Communication

- Wi-Fi Module: ESP8266-12
- Cloud Platform: ThingSpeak
- Data Sent:
  - Latitude
  - Longitude
  - Crack Detection Status
  - Obstacle Detection Status

##  Results

- Successfully detected track cracks using the IR sensor.
- Detected obstacles within the configured sensing range using the Ultrasonic sensor.
- Uploaded fault information to ThingSpeak in real time.
- Demonstrated a low-cost prototype for smart railway safety monitoring.

##  Future Enhancements

- AI/ML-based crack classification
- Camera-based vision detection
- GSM/LoRa communication for remote areas
- Solar-powered operation
- Integration with railway control systems
- Mobile application for instant notifications

