# CONTRIBUTING

個人の実験リポジトリだが、運用ルールを以下にまとめる。

## フォルダ構成のルール

- スケッチは `<回>/<テーマ名>/<テーマ名>.ino` の形で置く（Arduino IDE の制約上、フォルダ名と .ino 名を一致させる）
- `3rd/` のフェーズフォルダは `N-名前` 形式（例: `1-motor-test`, `3-ir-test`）
- **`.ino` を含むフォルダには必ず `WIRING.md`（部品・ピン接続表・配線概略図・注意事項）を同梱する**
- 設計に関わる変更（ピン割り当て・パラメータ・状態遷移）は `3rd/smart-trash-can/DESIGN.md` にも反映する

## コミット

- コミットメッセージは日本語で簡潔に（例: 「設計仕様変更」「サーボ」）
- レポート類（`docs/` の docx/pdf）はコード変更と分けてコミットする

## push 前のチェック

1. 変更したスケッチがコンパイルできること

   ```
   arduino-cli compile --fqbn arduino:avr:uno 3rd/<スケッチフォルダ>
   ```

2. `WIRING.md` / `DESIGN.md` / `README.md` の記載が実機・コードと一致していること

## 環境

- ボード: Arduino Uno（`arduino:avr:uno`）
- ライブラリ: IRremote v4.x, Servo, VarSpeedServo（第1回のみ）, SR04（該当フォルダに同梱）
