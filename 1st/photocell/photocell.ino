// フォトセル抵抗値測定
// 配線: 5V → 10kΩ抵抗 → A0 → フォトセル → GND

const int ANALOG_PIN = A0;
const float VCC = 5.0;
const float R_KNOWN = 10000.0;  // 10kΩ

void setup() {
  Serial.begin(9600);
  Serial.println("照度環境を整えてからキーボードで何かキーを押してください");
}

void loop() {
  if (Serial.available()) {
    Serial.read();  // 入力を読み捨て

    // 10回平均して安定させる
    long sum = 0;
    for (int i = 0; i < 10; i++) {
      sum += analogRead(ANALOG_PIN);
      delay(50);
    }
    float raw = sum / 10.0;

    float voltage = raw * VCC / 1023.0;
    float resistance = R_KNOWN * voltage / (VCC - voltage);

    Serial.print("ADC値: ");
    Serial.print(raw);
    Serial.print("  電圧: ");
    Serial.print(voltage, 3);
    Serial.print(" V  抵抗値: ");
    Serial.print(resistance, 0);
    Serial.println(" Ω");
    Serial.println("次の環境に切り替えたら何かキーを押してください");
  }
}
