# CLAUDE.md

情報通信実験B「センサと制御」の Arduino スケッチ・レポートを管理するリポジトリ。

## 構成

- `1st/`, `2nd/` — 第1・2回実験（完了。基本的に変更しない）
- `3rd/` — 最終制作「自走式自動開閉ごみ箱」。設計書は `3rd/smart-trash-can/DESIGN.md`
- `docs/` — レポート・データシート（バイナリ。編集しない）
- 残作業は `TODO.md` を参照

## ルール

- 新しい `.ino` を置くフォルダには必ず `WIRING.md`（配線図）を同梱する
- `3rd/` のフェーズフォルダ名は `N-名前` 形式（例: `3-ir-test`）。`phaseN-xxx` は使わない
- ドキュメント・コードコメントは日本語で書く
- ピン割り当てやパラメータを変えたら `DESIGN.md` と各 `WIRING.md` も必ず同期する
- DRV8835 の右モータは実機で **D5=BIN2 / D4=BIN1** に配線されている（設計初版と逆）。ドキュメント・コードはこれを正とする
- IRリモコンのボタンコード定数（`IR_FWD` 等）は `3-ir-test` での実測値を正とする

## ビルド確認

```
arduino-cli compile --fqbn arduino:avr:uno 3rd/<スケッチフォルダ>
```

必要ライブラリ: `IRremote`（v4.x）, `Servo`（`arduino-cli lib install IRremote Servo`）
