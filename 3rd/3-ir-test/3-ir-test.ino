#include <Arduino.h>
#define DECODE_NEC          // スターターキットのリモコンはNECフォーマット
#include <IRremote.hpp>     // IRremote v4.x（ライブラリマネージャで「IRremote」をインストール）

// IR受信ピン
#define IR_PIN  2

// リピートコードがこの時間途絶えたら「ボタンが離された」と判定
const unsigned long IR_REPEAT_TIMEOUT_MS = 200;

bool pressed = false;
uint8_t lastCmd = 0;
unsigned long lastIrTime = 0;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("IR test ready");
  Serial.println("ボタンを押すと PRESS、離すと RELEASE が表示される");
  Serial.println("cmd の値を smart-trash-can の IR_xxx 定数に転記する");
}

void loop() {
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
      // 長押し中は約110ms間隔でリピートコードが届く
      if (pressed) lastIrTime = millis();
    } else {
      lastCmd = IrReceiver.decodedIRData.command;
      pressed = true;
      lastIrTime = millis();
      Serial.print("PRESS   protocol=");
      Serial.print(getProtocolString(IrReceiver.decodedIRData.protocol));
      Serial.print(" addr=0x");
      Serial.print(IrReceiver.decodedIRData.address, HEX);
      Serial.print(" cmd=0x");
      Serial.println(lastCmd, HEX);
    }
    IrReceiver.resume();
  }

  // リピート途絶 → ボタン解放
  if (pressed && millis() - lastIrTime > IR_REPEAT_TIMEOUT_MS) {
    pressed = false;
    Serial.print("RELEASE cmd=0x");
    Serial.println(lastCmd, HEX);
  }
}
