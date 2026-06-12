#include <Arduino.h>
#define DECODE_NEC          // スターターキットのリモコンはNECフォーマット
#include <IRremote.hpp>     // IRremote v4.x

// ---- ピン定義 ----
#define IR_PIN    2
#define TRIG_PIN  12
#define ECHO_PIN  11
#define AIN1      7   // 左モータ 前進
#define AIN2      6   // 左モータ 後退（PWM）
#define BIN2      5   // 右モータ 前進（PWM）※実機配線に合わせD5=BIN2
#define BIN1      4   // 右モータ 後退　　※実機配線に合わせD4=BIN1

// ---- IRリモコンのボタンコード（3-ir-test の実測値に書き換える）----
const uint8_t IR_FWD   = 0x46;  // ▲ 前進
const uint8_t IR_BWD   = 0x15;  // ▼ 後退
const uint8_t IR_LEFT  = 0x44;  // ◀ 左旋回
const uint8_t IR_RIGHT = 0x43;  // ▶ 右旋回
const uint8_t IR_OK    = 0x40;  // OK 即時停止（保険用）

// ---- パラメータ ----
const int DRIVE_SPEED = 180;                       // 前進・後退PWM
const int TURN_SPEED  = 150;                       // 旋回PWM
const unsigned long IR_REPEAT_TIMEOUT_MS = 200;    // リピート途絶＝ボタン解放
const int STOP_DIST = 30;                          // cm: 前進中の自動停止距離
const unsigned long SONAR_INTERVAL_MS = 60;        // 距離計測周期

enum State { IDLE, MOVING };
State state = IDLE;

uint8_t currentCmd = 0;          // 走行中のコマンド（0=なし）
unsigned long lastIrTime = 0;
unsigned long lastSonarTime = 0;

// ---- モータ制御（spd: -255〜255、正=前進）----
// 左: AIN1=デジタル, AIN2=PWM / 右: BIN2=PWM, BIN1=デジタル のため
// 前進・後退でPWMのかけ方が異なる（詳細はWIRING.md）

void leftMotor(int spd) {
  if (spd > 0)      { digitalWrite(AIN1, HIGH); analogWrite(AIN2, 255 - spd); }
  else if (spd < 0) { digitalWrite(AIN1, LOW);  analogWrite(AIN2, -spd); }
  else              { digitalWrite(AIN1, LOW);  analogWrite(AIN2, 0); }
}

void rightMotor(int spd) {
  if (spd > 0)      { analogWrite(BIN2, spd);       digitalWrite(BIN1, LOW); }
  else if (spd < 0) { analogWrite(BIN2, 255 + spd); digitalWrite(BIN1, HIGH); }
  else              { analogWrite(BIN2, 0);         digitalWrite(BIN1, LOW); }
}

void driveStop() {
  leftMotor(0);
  rightMotor(0);
}

bool isDirection(uint8_t cmd) {
  return cmd == IR_FWD || cmd == IR_BWD || cmd == IR_LEFT || cmd == IR_RIGHT;
}

void startDrive(uint8_t cmd) {
  if      (cmd == IR_FWD)   { leftMotor(DRIVE_SPEED);  rightMotor(DRIVE_SPEED);  }
  else if (cmd == IR_BWD)   { leftMotor(-DRIVE_SPEED); rightMotor(-DRIVE_SPEED); }
  else if (cmd == IR_LEFT)  { leftMotor(-TURN_SPEED);  rightMotor(TURN_SPEED);   }
  else if (cmd == IR_RIGHT) { leftMotor(TURN_SPEED);   rightMotor(-TURN_SPEED);  }
  currentCmd = cmd;
  state = MOVING;
  lastIrTime = millis();
}

// ---- HC-SR04 ----
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

// ---- IR受信処理 ----
void handleIr() {
  if (!IrReceiver.decode()) return;

  if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
    // 長押し中のリピート：走行継続。タイムアウト誤判定で止まっていたら復帰
    if (state == MOVING) {
      lastIrTime = millis();
    } else if (state == IDLE && isDirection(currentCmd)) {
      startDrive(currentCmd);
    }
  } else {
    uint8_t cmd = IrReceiver.decodedIRData.command;
    if (isDirection(cmd)) {
      startDrive(cmd);
      Serial.print(">> MOVE cmd=0x");
      Serial.println(cmd, HEX);
    } else if (cmd == IR_OK) {
      driveStop();
      currentCmd = 0;
      state = IDLE;
      Serial.println(">> STOP (OK)");
    }
  }
  IrReceiver.resume();
}

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  driveStop();
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("Drive control ready");
}

void loop() {
  unsigned long now = millis();
  handleIr();

  if (state == MOVING) {
    // ボタン解放（リピート途絶）で停止
    if (now - lastIrTime > IR_REPEAT_TIMEOUT_MS) {
      driveStop();
      state = IDLE;
      Serial.println(">> STOP (release)");
      return;
    }
    // 前進中のみ接近自動停止
    if (currentCmd == IR_FWD && now - lastSonarTime >= SONAR_INTERVAL_MS) {
      lastSonarTime = now;
      long d = measureCm();
      if (d <= STOP_DIST) {
        driveStop();
        currentCmd = 0;  // 押しっぱなしのリピートで再発進しないようクリア
        state = IDLE;
        Serial.print(">> AUTO STOP dist=");
        Serial.print(d);
        Serial.println("cm");
      }
    }
  }
}
