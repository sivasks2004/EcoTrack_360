#include <Servo.h>

Servo myServo;  // Create a servo object

// Pin assignments
const int proximityPin = 2;          // Proximity sensor
const int soilMoisturePin = A0;      // Capacitive soil moisture sensor
const int trigPin = 10;              // Ultrasonic sensor trig pin
const int echoPin = 11;              // Ultrasonic sensor echo pin
const int thresholdMoisture = 400;   // Adjust based on sensor calibration
const int ultrasonicThreshold = 10; // Distance in cm for detecting objects

void setup() {
  Serial.begin(9600);
  pinMode(proximityPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  myServo.attach(9);  // Attach servo to pin 9
  myServo.write(90);   // Neutral position
  delay(2000);        // Wait for initial setup
}

long getUltrasonicDistance() {
  // Send a 10-microsecond pulse to trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the echo time in microseconds
  long duration = pulseIn(echoPin, HIGH);

  // Convert the time to distance in cm
  long distance = duration * 0.034 / 2;
  return distance;
}

void loop() {
  int proximityValue = digitalRead(proximityPin);
  int moistureValue = analogRead(soilMoisturePin);
  long distance = getUltrasonicDistance();

  // Print sensor values for debugging
  Serial.print("Proximity Value: ");
  Serial.println(proximityValue);
  Serial.print("Soil Moisture Value: ");
  Serial.println(moistureValue);
  Serial.print("Ultrasonic Distance: ");
  Serial.println(distance);

  // Decision based on sensor values
  if (moistureValue < thresholdMoisture) {
    Serial.println("Organic waste detected!");
    myServo.write(45);  // Move to organic side
  } else if (proximityValue == LOW) {
    Serial.println("Inorganic waste detected!");
    myServo.write(135);   // Move to inorganic side
  } else if (distance < ultrasonicThreshold) {
    Serial.println("Plastic waste detected by ultrasonic sensor!");
    myServo.write(135);   // Move to inorganic side
  } else {
    Serial.println("No waste detected!");
    myServo.write(90);  // Neutral position
  }

  delay(3000);  // Wait for waste to drop
  myServo.write(90);  // Reset servo to neutral position
  delay(500);  // Small delay to avoid repeated detections
}