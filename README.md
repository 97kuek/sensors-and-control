# センサと制御

情報通信実験B「センサと制御」で用いたプログラム・資料をまとめたリポジトリ。

## ディレクトリ構成

```
.
├── 1st/                        # 第1回実験
│   ├── led-voltage/            # LED順方向電圧の測定
│   ├── photocell/              # フォトセル抵抗値の測定
│   ├── servo/                  # サーボモータ角速度の測定
│   └── ultrasonic-sensor/      # 超音波センサによる距離測定
├── 2nd/                        # 第2回実験（予定）
├── 3rd/                        # 第3回実験（予定）
└── docs/                       # レポート・資料
    ├── 各電子部品のデータシート/
    ├── データシート.xlsx
    ├── センサと制御実験キットについて.pdf
    ├── 最終制作物についてのガイドライン.pdf
    └── センサと制御第1回レポート.pdf
```

## 実験内容

### 第1回

| スケッチ | 概要 |
|---|---|
| [led-voltage.ino](1st/led-voltage/led-voltage.ino) | 赤・黄・緑・青・白の各LEDを点灯し、マルチメーターで順方向電圧を測定する |
| [photocell.ino](1st/photocell/photocell.ino) | フォトセルと10kΩ抵抗による分圧回路でADC値を読み取り、照度環境ごとの抵抗値を算出する |
| [servo.ino](1st/servo/servo.ino) | サーボモータを0°↔180°往復させ、10回平均から角速度 (rad/s) を求める |
| [ultrasonic-sensor.ino](1st/ultrasonic-sensor/ultrasonic-sensor.ino) | HC-SR04超音波センサで1秒ごとに距離 (cm) をシリアル出力する |

## 環境

- マイコンボード: Arduino
- IDE: Arduino IDE
- 使用ライブラリ
  - [VarSpeedServo](https://github.com/netlabtoolkit/VarSpeedServo) — サーボ速度制御
  - SR04 — HC-SR04超音波センサドライバ（`1st/ultrasonic-sensor/` に同梱）
