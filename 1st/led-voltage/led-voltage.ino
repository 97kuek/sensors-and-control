// 各LEDを点灯させて順方向電圧をマルチメーターで測定する
// 配線: 各ピン → 220Ω抵抗 → LED(+) → LED(-) → GND

const int RED_PIN   = 2;
const int YELLOW_PIN = 3;
const int GREEN_PIN  = 4;
const int BLUE_PIN   = 5;
const int WHITE_PIN  = 6;

void setup() {
  pinMode(RED_PIN,    OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN,  OUTPUT);
  pinMode(BLUE_PIN,   OUTPUT);
  pinMode(WHITE_PIN,  OUTPUT);

  digitalWrite(RED_PIN,    HIGH);
  digitalWrite(YELLOW_PIN, HIGH);
  digitalWrite(GREEN_PIN,  HIGH);
  digitalWrite(BLUE_PIN,   HIGH);
  digitalWrite(WHITE_PIN,  HIGH);
}

void loop() {}
