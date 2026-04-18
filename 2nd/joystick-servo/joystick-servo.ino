#include <Servo.h>

#define JOY_X_PIN A0
#define SERVO_PIN 9

Servo myServo;

void setup() {
  myServo.attach(SERVO_PIN);
  Serial.begin(9600);
}

void loop() {
  int raw = analogRead(JOY_X_PIN);
  int angle = map(raw, 0, 1023, 0, 180);
  myServo.write(angle);

  Serial.print("X: ");
  Serial.print(raw);
  Serial.print(" -> ");
  Serial.print(angle);
  Serial.println(" deg");

  delay(20);
}