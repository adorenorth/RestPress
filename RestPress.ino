// RestPress™ Prototype v1.0
// Controls two side-mounted electromagnets via PWM
// Author: Erin-Leigh Gallop
// License: All rights reserved. For demonstration purposes only.

#define LEFT_COIL_PIN  5   // PWM-capable pin for left magnet
#define RIGHT_COIL_PIN 6   // PWM-capable pin for right magnet
#define TEMP_SENSOR_PIN A0 // Analog pin for thermal sensor (e.g., 10k NTC thermistor)

const int MAX_TEMP_C = 50;         // Max safe temperature in Celsius
const int PWM_DUTY_CYCLE = 180;    // PWM value (0–255); tweak for comfort level
const unsigned long RUNTIME_MS = 7UL * 60UL * 60UL * 1000UL;  // 7 hours in ms

unsigned long startTime;

void setup() {
  pinMode(LEFT_COIL_PIN, OUTPUT);
  pinMode(RIGHT_COIL_PIN, OUTPUT);
  analogWrite(LEFT_COIL_PIN, 0);
  analogWrite(RIGHT_COIL_PIN, 0);
  startTime = millis();

  Serial.begin(9600);
}

void loop() {
  float tempC = readTemperatureC();

  if (tempC > MAX_TEMP_C) {
    // Safety cutoff
    analogWrite(LEFT_COIL_PIN, 0);
    analogWrite(RIGHT_COIL_PIN, 0);
    Serial.println("Temperature too high! Coils turned off.");
  } else if (millis() - startTime <= RUNTIME_MS) {
    // Normal operation
    analogWrite(LEFT_COIL_PIN, PWM_DUTY_CYCLE);
    analogWrite(RIGHT_COIL_PIN, PWM_DUTY_CYCLE);
    Serial.print("Temp: "); Serial.print(tempC); Serial.println("°C");
  } else {
    // Time's up — turn off
    analogWrite(LEFT_COIL_PIN, 0);
    analogWrite(RIGHT_COIL_PIN, 0);
    Serial.println("Timer complete. Coils turned off.");
  }

  delay(1000); // Check every second
}

float readTemperatureC() {
  int analogValue = analogRead(TEMP_SENSOR_PIN);
  float voltage = analogValue * (5.0 / 1023.0);
  float resistance = (5.0 - voltage) * 10000 / voltage; // Assuming 10k fixed resistor

  // Simple Steinhart-Hart for 10k NTC thermistor
  float tempK = 1.0 / (0.001129148 + 0.000234125 * log(resistance) + 0.0000000876741 * pow(log(resistance), 3));
  return tempK - 273.15;
}
