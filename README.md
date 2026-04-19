# センサと制御

情報通信実験B「センサと制御」で用いたプログラム・資料をまとめたリポジトリ。

- [センサと制御](#センサと制御)
  - [ディレクトリ構成](#ディレクトリ構成)
  - [実験内容](#実験内容)
    - [第1回中間レポート](#第1回中間レポート)
    - [第2回中間レポート](#第2回中間レポート)
    - [最終レポート — 音で呼べる自動開閉ごみ箱](#最終レポート--音で呼べる自動開閉ごみ箱)
  - [環境](#環境)

## ディレクトリ構成

```
.
├── 1st/                          # 第1回実験
│   ├── led-voltage/              # LED順方向電圧の測定
│   ├── pencil-resistance/        # 鉛筆線の距離-抵抗特性
│   ├── photocell/                # フォトセル抵抗値の測定
│   ├── servo/                    # サーボモータ角速度の測定
│   └── ultrasonic-sensor/        # 超音波センサによる距離測定
├── 2nd/                          # 第2回実験
│   ├── joystick-servo/           # ジョイスティックでサーボを制御
│   └── ultrasonic-buzzer/        # 超音波センサで音程を制御
├── assets/                       # グラフ画像
├── docs/                         # レポート・資料
│   ├── 各電子部品のデータシート/
│   ├── データシート.xlsx
│   ├── センサと制御実験キットについて.pdf
│   ├── 最終制作物についてのガイドライン.pdf
│   ├── センサと制御第1回レポート.pdf
│   ├── センサと制御第2回レポート.pdf
│   ├── 1W242038_植木敬太郎_センサと制御_第1回中間.pdf
│   └── 1W242038_植木敬太郎_センサと制御_第2回中間.pdf
└── 3rd/                          # 第3回実験（最終制作）
    ├── 1-motor-test/             # Phase 1: DCモータ単体テスト
    └── smart-trash-can/          # 最終統合（設計書）
```

## 実験内容

### 第1回中間レポート

| ファイル | 概要 |
|---|---|
| [led-voltage.ino](1st/led-voltage/led-voltage.ino) | 赤・黄・緑・青・白の各LEDを点灯し、テスターで順方向電圧を測定する |
| [photocell.ino](1st/photocell/photocell.ino) | フォトセルと10kΩ抵抗による分圧回路でADC値を読み取り、照度環境ごとの抵抗値を算出する |
| [servo.ino](1st/servo/servo.ino) | サーボモータを0°から180°まで往復させ、10回平均から角速度 (rad/s) を求める |
| [ultrasonic-sensor.ino](1st/ultrasonic-sensor/ultrasonic-sensor.ino) | HC-SR04超音波センサで1秒ごとに距離をシリアル出力する |
| [plot_distance.m](1st/ultrasonic-sensor/plot_distance.m) | 超音波センサの実測値と理想値を比較する |
| [plot_pencil.m](1st/pencil-resistance/plot_pencil.m) | シャープペンシル線（HB）の2点間距離と抵抗値の関係をプロットする |

### 第2回中間レポート

| ファイル | 概要 |
|---|---|
| [joystick-servo.ino](2nd/joystick-servo/joystick-servo.ino) | ジョイスティックのX軸でサーボ角度を0°〜180°で制御する |
| [ultrasonic-buzzer.ino](2nd/ultrasonic-buzzer/ultrasonic-buzzer.ino) | 超音波センサで距離を測定し（〜20cm）、近いほど高音をパッシブブザーで鳴らす |

### 最終レポート — 音で呼べる自動開閉ごみ箱

手を叩くとごみ箱が方向を判定して自走し、近づいたら自動でフタが開閉するシステム。

**動作フロー**

```
拍手検知 → 方向判定 → 旋回・走行 → 距離監視 → 停止 → 手かざし検知 → フタ開閉
```

**実装フェーズ**

| フェーズ | フォルダ | 内容 | 状態 |
|---|---|---|---|
| Phase 1 | [1-motor-test/](3rd/1-motor-test/) | DCモータ単体テスト（前進・旋回・停止） | 実装済み |
| Phase 2 | `2-drive-with-stop/` | 超音波センサで距離監視→走行停止 | 未着手 |
| Phase 3 | `3-lid-control/` | 手かざし検知→サーボでフタ開閉 | 未着手 |
| Phase 4 | `4-mic-test/` | マイク3ch サンプリング・ピーク確認 | 未着手 |
| Phase 5 | `5-direction-detect/` | ピーク比較による方向判定 | 未着手 |
| Phase 6 | `smart-trash-can/` | 全要素統合・最終調整 | 未着手 |

設計の詳細は [DESIGN.md](3rd/smart-trash-can/DESIGN.md) を参照。

**主要部品**

| 部品 | 数量 |
|---|---|
| Arduino Uno | 1 |
| MAX4466搭載アナログマイクモジュール | 3 |
| HC-SR04 超音波センサ | 1 |
| SG90 サーボモータ | 1 |
| TT ギアモータ（DCモータ） | 2 |
| DRV8835 デュアルモータドライバ | 1 |

## 環境

- マイコンボード: Arduino UNO
- IDE: Arduino IDE
- 使用ライブラリ
  - [VarSpeedServo](https://github.com/netlabtoolkit/VarSpeedServo) — サーボ速度制御（第1回）
  - SR04 — HC-SR04超音波センサドライバ（`1st/ultrasonic-sensor/` および `2nd/ultrasonic-buzzer/` に同梱）
