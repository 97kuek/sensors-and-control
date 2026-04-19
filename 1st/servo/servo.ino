#include <VarSpeedServo.h>

VarSpeedServo myservo;

void setup() {
  Serial.begin(9600);
  myservo.attach(9);

  // 0°に移動
  myservo.write(0, 255, true);                  // myservo.write(角度, 速度, 待機), 第3引数をtrueにすると指定の角度に到達するまで次の行の命令を待つ
  delay(500);

  // 10回計測して平均を取る
  const int N = 10;
  unsigned long total = 0;
  for (int i = 0; i < N; i++) {
    unsigned long t = millis();                 // 計測開始時の時刻を記録
    myservo.write(180, 255, true);              // 180度へ最高速で移動（完了まで待機）
    total += millis() - t;                      // 移動にかかった時間をtotalに足す 

    myservo.write(0, 255, true);                // 次の計測のため0度に戻す
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
