// ピン設定
const int RED_PIN   = 2;
const int YELLOW_PIN = 3;
const int GREEN_PIN  = 4;
const int BLUE_PIN   = 5;
const int WHITE_PIN  = 6;

void setup() {
  // LEDのピンを出力に設定
  pinMode(RED_PIN,    OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN,  OUTPUT);
  pinMode(BLUE_PIN,   OUTPUT);
  pinMode(WHITE_PIN,  OUTPUT);

  // 全てのLEDを点灯
  digitalWrite(RED_PIN,    HIGH);
  digitalWrite(YELLOW_PIN, HIGH);
  digitalWrite(GREEN_PIN,  HIGH);
  digitalWrite(BLUE_PIN,   HIGH);
  digitalWrite(WHITE_PIN,  HIGH);
}

void loop() {}
