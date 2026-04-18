#include <VarSpeedServo.h>

VarSpeedServo myservo;

void setup() {
  Serial.begin(9600);
  myservo.attach(9);

  // 0°に移動して安定させる
  myservo.write(0, 255, true);
  delay(500);

  // 10回計測して平均を取る
  const int N = 10;
  unsigned long total = 0;

  for (int i = 0; i < N; i++) {
    unsigned long t = millis();
    myservo.write(180, 255, true);  // 最大速度で180°、完了まで待機
    total += millis() - t;

    myservo.write(0, 255, true);    // 0°に戻す
    delay(200);
  }

  unsigned long avgMs = total / N;
  float omega = 3.14159 / (avgMs / 1000.0);  // rad/s

  Serial.print("Average time for 180deg: ");
  Serial.print(avgMs);
  Serial.println(" ms");

  Serial.print("Angular velocity: ");
  Serial.print(omega);
  Serial.println(" rad/s");
}

void loop() {}
