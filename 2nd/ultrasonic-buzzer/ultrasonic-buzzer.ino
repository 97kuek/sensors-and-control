#include "SR04.h"

// ピン設定
#define TRIG_PIN  12
#define ECHO_PIN  11
#define BUZZER_PIN 8

// 距離の有効範囲 (cm)
const int DIST_MIN = 2;
const int DIST_MAX = 20;

// 音程の範囲 (Hz)
const int FREQ_MIN = 200;
const int FREQ_MAX = 2000;

const int SMOOTH = 5;                                              // 平均する回数

SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);                                     // 8番ピンをブザーの出力に設定
  Serial.begin(9600);
}

void loop() {

  // 距離を複数回測定して平均を取る
  long sum = 0;
  for (int i = 0; i < SMOOTH; i++) {
    sum += sr04.Distance();                                       // 現在の合計にセンサーで測った距離を足していく
    delay(10);
  }
  long dist = constrain(sum / SMOOTH, DIST_MIN, DIST_MAX);        // constrain(値, 最小, 最大)は値を範囲内に収める関数

  // 距離を周波数に変換
  int freq = map(dist, DIST_MIN, DIST_MAX, FREQ_MAX, FREQ_MIN);   // map(値, 入力最小, 入力最大, 出力最小, 出力最大)は値を別の範囲に変換する関数
  tone(BUZZER_PIN, freq);                                         // ブザーから周波数freqの音を鳴らす              

  // シリアルモニタに距離と周波数を表示
  Serial.print("Distance: ");
  Serial.print(dist);
  Serial.print(" cm  ->  ");
  Serial.print(freq);
  Serial.println(" Hz");

  delay(30);
}
