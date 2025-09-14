#include <Servo.h>
#include <Wire.h>
#include <LCD_I2C.h> // Include the LCD_I2C library

const int triggerPin = 8;  // Pin connected to the Trigger of HC-SR04
const int echoPin = 7;     // Pin connected to the Echo of HC-SR04
const int servoPin = 9;    // Pin connected to the Servo motor

Servo dustbinServo;        // Create a Servo object to control the dustbin lid
LCD_I2C lcd(0x3F);         // Initialize the LCD (I2C address 0x3F)

long duration;
int distance;

void setup() {
  // Start the serial monitor
  Serial.begin(9600);
  
  // Set up pins for ultrasonic sensor
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Attach the servo to the servoPin
  dustbinServo.attach(servoPin);

  // Initialize the LCD
  lcd.begin();           // Initialize LCD
  lcd.backlight();       // Turn on LCD backlight
  lcd.print("Smart Dustbin"); // Display initial message

  // Initially close the dustbin lid
  dustbinServo.write(0);  // 0 degrees - closed position
  delay(2000);  // Display initial message for 2 seconds
  lcd.clear();
}

void loop() {
  // Trigger the ultrasonic sensor to start measurement
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  // Read the echo time and calculate the distance
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;  // Calculate distance in cm
  
  // Print distance for debugging
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // If movement is detected within 30 cm
  if (distance < 30) {
    // Open the dustbin lid (servo to 90 degrees)
    dustbinServo.write(90);
    Serial.println("Movement detected! Opening dustbin...");
    lcd.clear();
    lcd.print("Lid Open");

    // Countdown for closing the lid
    for (int seconds = 30; seconds >= 0; seconds--) {
      lcd.setCursor(0, 1);
      lcd.print("Closing in: ");
      lcd.print(seconds);
      lcd.print("s  "); // Ensure trailing spaces clear the display
      
      // Check for movement during countdown
      digitalWrite(triggerPin, LOW);
      delayMicroseconds(2);
      digitalWrite(triggerPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(triggerPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distance = duration * 0.034 / 2;

      if (distance < 30) {
        Serial.println("Movement detected during countdown! Pausing...");
        lcd.clear();
        lcd.print("Open for : 30s");

        // Pause countdown for 30 seconds
        delay(30000);

        // Resume countdown message
        lcd.clear();
        lcd.print("Resuming...");
        delay(2000);
        lcd.clear();
        lcd.print("Lid Open");
      }

      delay(1000); // Wait for 1 second
    }
    
    // Close the dustbin lid (servo back to 0 degrees)
    dustbinServo.write(0);
    Serial.println("Closing dustbin.");
    lcd.clear();
    lcd.print("Lid Closed");
    delay(2000); // Show "Lid Closed" for 2 seconds
    lcd.clear();
  }
  
  // Add a small delay to avoid continuous triggering
  delay(500);
}
