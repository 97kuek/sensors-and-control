#include <Arduino.h>

#include <Servo.h>



// HC-SR04

#define TRIG_PIN  12

#define ECHO_PIN  11



// サーボ

#define SERVO_PIN  9



const int HAND_DIST     = 5;    // cm: フタを開ける距離

const int OPEN_DURATION = 3000;  // ms: フタ開放時間

const int ANGLE_OPEN    = 180;    // サーボ全開角度

const int ANGLE_CLOSED  = 40;     // サーボ閉角度



Servo lidServo;



long measureCm() {

  digitalWrite(TRIG_PIN, LOW);

  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);

  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);  // 30ms タイムアウト

  if (duration == 0) return 999;  // 測定範囲外

  return duration / 58;

}



void setup() {

  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);

  pinMode(ECHO_PIN, INPUT);

  lidServo.attach(SERVO_PIN);

  lidServo.write(ANGLE_CLOSED);

  Serial.println("Lid control ready");

}



void loop() {

  long dist = measureCm();

  Serial.print("dist=");

  Serial.print(dist);

  Serial.println("cm");



  if (dist <= HAND_DIST) {

    Serial.println(">> Hand detected: OPEN");

    lidServo.write(ANGLE_OPEN);

    delay(OPEN_DURATION);

    Serial.println(">> CLOSE");

    lidServo.write(ANGLE_CLOSED);

    delay(500);  // 連続検知防止

  }



  delay(100);

}
