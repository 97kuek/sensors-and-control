#include <Servo.h>

// ピン設定
#define JOY_X_PIN A0
#define SERVO_PIN 9

// インスタンス作成
Servo myServo;

void setup() {
  myServo.attach(SERVO_PIN);              // 9番ピンにサーボが繋がっていることを伝える
  Serial.begin(9600);
}

void loop() {
  int raw = analogRead(JOY_X_PIN);        // ジョイスティックのX軸の値を0~1023で読み取る
  int angle = map(raw, 0, 1023, 0, 180);  // map(値, 入力最小, 入力最大, 出力最小, 出力最大)は値を別の範囲に変換する関数
  myServo.write(angle);                   // サーボをangleの角度に動かす

  // シリアルモニタにジョイスティックの値とサーボの角度を表示
  Serial.print("X: ");
  Serial.print(raw);
  Serial.print(" -> ");
  Serial.print(angle);
  Serial.println(" deg");

  delay(20);
}