#include <Arduino.h>

// DRV8835 ピン定義
#define AIN1  7   // 左モータ 前進
#define AIN2  6   // 左モータ 後退（PWM）
#define BIN1  5   // 右モータ 前進（PWM）
#define BIN2  4   // 右モータ 後退

const int SPEED      = 180;  // 前進速度 (0-255)
const int TURN_SPEED = 150;  // 旋回速度 (0-255)

// ---- モータ制御関数 ----

void motorStop() {
  digitalWrite(AIN1, LOW); analogWrite(AIN2, 0);
  digitalWrite(BIN2, LOW); analogWrite(BIN1, 0);
}

void motorForward(int spd) {
  digitalWrite(AIN1, HIGH); analogWrite(AIN2, 0);
  analogWrite(BIN1, spd);   digitalWrite(BIN2, LOW);
}

// 右旋回：左前進・右停止
void motorTurnRight(int spd) {
  digitalWrite(AIN1, HIGH); analogWrite(AIN2, 0);
  analogWrite(BIN1, 0);     digitalWrite(BIN2, LOW);
}

// 左旋回：左停止・右前進
void motorTurnLeft(int spd) {
  digitalWrite(AIN1, LOW); analogWrite(AIN2, 0);
  analogWrite(BIN1, spd);  digitalWrite(BIN2, LOW);
}

// ---- セットアップ ----

void setup() {
  Serial.begin(9600);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  motorStop();
  Serial.println("Motor test start");
  delay(2000);  // 準備時間
}

// ---- テストシーケンス ----

void loop() {
  Serial.println(">> Forward 2s");
  motorForward(SPEED);
  delay(2000);

  motorStop();
  delay(500);

  Serial.println(">> Turn Right 1s");
  motorTurnRight(TURN_SPEED);
  delay(1000);

  motorStop();
  delay(500);

  Serial.println(">> Turn Left 1s");
  motorTurnLeft(TURN_SPEED);
  delay(1000);

  motorStop();
  Serial.println(">> Stop 3s");
  delay(3000);
}
