#include <Servo.h>

Servo myServo;  // Create a servo object

// Pin assignments
const int proximityPin = 2;          // Proximity sensor
const int soilMoisturePin = A0;      // Capacitive soil moisture sensor
const int thresholdMoisture = 400;  // Adjust based on sensor calibration

void setup() {
  Serial.begin(9600);
  pinMode(proximityPin, INPUT);

  myServo.attach(9);  // Attach servo to pin 9
  myServo.write(90);   // Neutral position
  delay(2000);        // Wait for initial setup
}

void loop() {
  int proximityValue = digitalRead(proximityPin);
  int moistureValue = analogRead(soilMoisturePin);

  // Print sensor values for debugging
  Serial.print("Proximity Value: ");
  Serial.println(proximityValue);
  Serial.print("Soil Moisture Value: ");
  Serial.println(moistureValue);

  // Decision based on sensor values
  if (moistureValue < thresholdMoisture) {
    Serial.println("Organic waste detected!");
    myServo.write(45);  // Move to organic side
  } else if (proximityValue == LOW) {
    Serial.println("Inorganic waste detected!");
    myServo.write(135);   // Move to inorganic side
  } else {
    Serial.println("No waste detected!");
    myServo.write(90);  // Neutral position
  }

  delay(3000);  // Wait for waste to drop
  myServo.write(90);  // Reset servo to neutral position
  delay(500);  // Small delay to avoid repeated detections
}
