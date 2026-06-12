# センサと制御

情報通信実験B「センサと制御」で用いたプログラム・資料をまとめたリポジトリ。

- [センサと制御](#センサと制御)
  - [ディレクトリ構成](#ディレクトリ構成)
  - [実験内容](#実験内容)
    - [第1回中間レポート](#第1回中間レポート)
    - [第2回中間レポート](#第2回中間レポート)
    - [最終レポート — 自走式自動開閉ごみ箱](#最終レポート--自走式自動開閉ごみ箱)
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
├── 3rd/                          # 第3回実験（最終制作）
│   ├── 1-motor-test/             # Phase 1: DCモータ単体テスト
│   ├── 2-lid-control/            # Phase 2: 手かざし検知→フタ開閉
│   ├── 3-ir-test/                # Phase 3: IRリモコン受信・長押し検証
│   ├── 4-drive-control/          # Phase 4: 長押し走行制御＋接近自動停止
│   └── smart-trash-can/          # Phase 5: 全統合（設計書 DESIGN.md）
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
└── TODO.md                       # 最終制作の残作業
```

> 各フェーズフォルダの `WIRING.md` に配線図を記載している。

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

### 最終レポート — 自走式自動開閉ごみ箱

IRリモコンで操作すると自走して近づき、フタが自動で開閉するごみ箱。「ごみ箱の方に行く」のではなく「ごみ箱が来てくれる」という発想の転換が特徴。

**動作フロー**

```
ボタン長押し → 走行（押している間だけ） → ユーザーに接近 → 自動停止 → 手かざし → フタ開閉
```

- リモコンの方向ボタンを**押している間だけ**走行し、離すと停止する（NECリピートコードの途絶検知）
- 前進中は HC-SR04 でユーザーとの距離を監視し、30cm 以内で自動停止する
- 停止後、同じ HC-SR04 で手かざしを検知してサーボでフタを開閉する
- 電源は Arduino 用とモータ用で分離し、電圧降下によるセンサ誤動作を防止する

**実装フェーズ**

| フェーズ | フォルダ | 内容 | 状態 |
|---|---|---|---|
| Phase 1 | [1-motor-test/](3rd/1-motor-test/) | DCモータ単体テスト（前進・旋回・停止） | 完了 |
| Phase 2 | [2-lid-control/](3rd/2-lid-control/) | 手かざし検知→サーボでフタ開閉 | 完了 |
| Phase 3 | [3-ir-test/](3rd/3-ir-test/) | IRリモコン受信コード確認・長押し処理 | コード作成済み |
| Phase 4 | [4-drive-control/](3rd/4-drive-control/) | IR長押し→走行制御＋接近自動停止 | コード作成済み |
| Phase 5 | [smart-trash-can/](3rd/smart-trash-can/) | 全要素統合・最終調整 | コード作成済み |

設計の詳細は [DESIGN.md](3rd/smart-trash-can/DESIGN.md)、残作業は [TODO.md](TODO.md) を参照。

> **注意:** スケッチ内のIRボタンコード（`IR_FWD` 等）は仮値。配線前に `3-ir-test` で実測して転記する（手順は [TODO.md](TODO.md)）。

**主要部品**

| 部品 | 数量 |
|---|---|
| Arduino Uno | 1 |
| VS1838B IR受信モジュール ＋ IRリモコン | 各1 |
| HC-SR04 超音波センサ | 1 |
| SG90 サーボモータ | 1 |
| TT ギアモータ（DCモータ） | 2 |
| DRV8835 デュアルモータドライバ | 1 |
| 電池ボックス（単3×4本・モータ用）／9V電池（Arduino用） | 各1 |

## 環境

- マイコンボード: Arduino UNO
- IDE: Arduino IDE（または arduino-cli）
- 使用ライブラリ
  - [IRremote](https://github.com/Arduino-IRremote/Arduino-IRremote) v4.x — IRリモコン受信（第3回）
  - Servo — サーボ制御（Arduino IDE 標準）
  - [VarSpeedServo](https://github.com/netlabtoolkit/VarSpeedServo) — サーボ速度制御（第1回）
  - SR04 — HC-SR04超音波センサドライバ（`1st/ultrasonic-sensor/` および `2nd/ultrasonic-buzzer/` に同梱）
