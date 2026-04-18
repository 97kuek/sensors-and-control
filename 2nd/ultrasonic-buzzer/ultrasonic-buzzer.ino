#include "SR04.h"

#define TRIG_PIN  12
#define ECHO_PIN  11
#define BUZZER_PIN 8

// 距離の有効範囲 (cm)
const int DIST_MIN = 2;
const int DIST_MAX = 20;

// 音程の範囲 (Hz): 近い→高音、遠い→低音
const int FREQ_MIN = 200;
const int FREQ_MAX = 2000;

const int SMOOTH = 5;  // 平均する回数

SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  long sum = 0;
  for (int i = 0; i < SMOOTH; i++) {
    sum += sr04.Distance();
    delay(10);
  }
  long dist = constrain(sum / SMOOTH, DIST_MIN, DIST_MAX);

  int freq = map(dist, DIST_MIN, DIST_MAX, FREQ_MAX, FREQ_MIN);
  tone(BUZZER_PIN, freq);

  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.print(" cm  ->  ");
  Serial.print(freq);
  Serial.println(" Hz");

  delay(30);
}
