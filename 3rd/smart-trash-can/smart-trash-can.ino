// 自走式自動開閉ごみ箱 — 統合版
// IRリモコン長押しで走行、接近で自動停止、手かざしでフタ開閉

#include <Arduino.h>
#include <Servo.h>
#define DECODE_NEC          // スターターキットのリモコンはNECフォーマット
#include <IRremote.hpp>     // IRremote v4.x

// ---- ピン定義 ----
#define IR_PIN     2
#define TRIG_PIN   12
#define ECHO_PIN   11
#define SERVO_PIN  9
#define AIN1       7   // 左モータ 前進
#define AIN2       6   // 左モータ 後退（PWM）
#define BIN2       5   // 右モータ 前進（PWM）※実機配線に合わせD5=BIN2
#define BIN1       4   // 右モータ 後退　　※実機配線に合わせD4=BIN1

// ---- IRリモコンのボタンコード（3-ir-test の実測値に書き換える）----
const uint8_t IR_FWD   = 0x46;  // ▲ 前進
const uint8_t IR_BWD   = 0x15;  // ▼ 後退
const uint8_t IR_LEFT  = 0x44;  // ◀ 左旋回
const uint8_t IR_RIGHT = 0x43;  // ▶ 右旋回
const uint8_t IR_OK    = 0x40;  // OK 即時停止（保険用）

// ---- パラメータ ----
const int DRIVE_SPEED = 180;                        // 前進・後退PWM
const int TURN_SPEED  = 150;                        // 旋回PWM
const unsigned long IR_REPEAT_TIMEOUT_MS = 200;     // リピート途絶＝ボタン解放
const int STOP_DIST = 30;                           // cm: 前進中の自動停止距離
const int HAND_DIST = 5;                            // cm: 手かざし検知距離
const unsigned long SONAR_INTERVAL_MS   = 60;       // 距離計測周期
const unsigned long WAITING_TIMEOUT_MS  = 30000;    // 手かざし待ちタイムアウト
const unsigned long OPEN_DURATION       = 3000;     // フタ開放時間
const int ANGLE_OPEN   = 160;                       // フタ全開角度（2-lid-controlで調整済み）
const int ANGLE_CLOSED = 40;                        // フタ閉角度

// ---- ステートマシン ----
enum State { IDLE, MOVING, WAITING, LID_OPEN };
State state = IDLE;

Servo lidServo;
uint8_t currentCmd = 0;          // 走行中のコマンド（0=なし）
unsigned long lastIrTime = 0;    // 最後にIR（リピート含む）を受信した時刻
unsigned long lastSonarTime = 0; // 最後に距離計測した時刻
unsigned long stateTime = 0;     // WAITING/LID_OPEN に入った時刻

// ---- モータ制御（spd: -255〜255、正=前進）----
// 左: AIN1=デジタル, AIN2=PWM / 右: BIN2=PWM, BIN1=デジタル のため
// 前進・後退でPWMのかけ方が異なる

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
      // フタが開いていたら閉じてから走行（開けたまま走らない）
      if (state == WAITING || state == LID_OPEN) {
        lidServo.write(ANGLE_CLOSED);
      }
      startDrive(cmd);
      Serial.print(">> MOVE cmd=0x");
      Serial.println(cmd, HEX);
    } else if (cmd == IR_OK) {
      driveStop();
      lidServo.write(ANGLE_CLOSED);
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
  lidServo.attach(SERVO_PIN);
  lidServo.write(ANGLE_CLOSED);
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("Smart trash can ready");
}

void loop() {
  unsigned long now = millis();
  handleIr();

  switch (state) {
    case MOVING:
      // ボタン解放（リピート途絶）で停止
      if (now - lastIrTime > IR_REPEAT_TIMEOUT_MS) {
        driveStop();
        state = IDLE;
        Serial.println(">> STOP (release)");
        break;
      }
      // 前進中のみ接近自動停止 → 手かざし待ちへ
      if (currentCmd == IR_FWD && now - lastSonarTime >= SONAR_INTERVAL_MS) {
        lastSonarTime = now;
        long d = measureCm();
        if (d <= STOP_DIST) {
          driveStop();
          currentCmd = 0;  // 押しっぱなしのリピートで再発進しないようクリア
          state = WAITING;
          stateTime = now;
          Serial.print(">> AUTO STOP dist=");
          Serial.print(d);
          Serial.println("cm -> WAITING");
        }
      }
      break;

    case WAITING:
      // タイムアウトで待機解除
      if (now - stateTime > WAITING_TIMEOUT_MS) {
        state = IDLE;
        Serial.println(">> WAITING timeout");
        break;
      }
      // 手かざし検知でフタ開
      if (now - lastSonarTime >= SONAR_INTERVAL_MS) {
        lastSonarTime = now;
        long d = measureCm();
        if (d <= HAND_DIST) {
          lidServo.write(ANGLE_OPEN);
          state = LID_OPEN;
          stateTime = now;
          Serial.println(">> LID OPEN");
        }
      }
      break;

    case LID_OPEN:
      // 一定時間でフタを閉めて待機状態へ
      if (now - stateTime > OPEN_DURATION) {
        lidServo.write(ANGLE_CLOSED);
        state = IDLE;
        Serial.println(">> LID CLOSE");
      }
      break;

    case IDLE:
    default:
      break;
  }
}
