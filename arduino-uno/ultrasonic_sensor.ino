#include <SoftwareSerial.h>

// Communication pins to ESP32 (RX= Pin 2, TX = Pin 3)
SoftwareSerial espSerial(2, 3);

// Ultrasonic sensor pins
const int trigPin = 9;
const int echoPin = 10;

void setup() {
  espSerial.begin(9600);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Clear trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Send 10ms pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read duration
  long pulseDuration = pulseIn(echoPin, HIGH);

  // Calculate distance in centimeters
  float calculatedDistance = (pulseDuration * 0.0343) / 2.0;

  // Send result to ESP32
  espSerial.println(calculatedDistance, 1); 

  // Wait half a second before next reading
  delay(500);
}
