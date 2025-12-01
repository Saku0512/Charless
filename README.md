# Charless

- 拡張子は `.cless`
- 使用する文字は 0 から 9 の数字のみ。

## 特別な数字（予約語）
- 20: トークン（単語）間の区切り文字 (スペースの役割)
- 99: 「数値リテラル」モードの開始を宣言するプレフィックス

## 数値リテラルの表現 (99)
数値をコードの一部として埋め込む方法。

1.  `99` トークンで「数値リテラル」モードを開始する。
2.  `99` の直後に続く数字の並びは、次に出現する `20` (区切り) まで、すべて1つの「数値」として解釈される。
3.  このモード中は、`20` 以外の数字（`99` や他の命令番号を含む）はすべて数値の一部として扱われる。

## 文字列 (連続可変長データ) の表現
特定の命令（例: `01` = 出力命令）に続くデータとして定義する。

1.  形式は「長さプレフィックス」方式を採用する。
2.  フォーマット: `[桁数][データ本体]`
    - `[桁数]` : `[データ本体]` の桁数を表す数字。一バイトずつ処理するため一桁固定。
    - `[データ本体]` : 実際のデータ（文字コードなど）。
3.  この `[桁数][データ本体]` のペアは、次の `20` (区切り) が出現するまで、連続して並べることができる。
4.  データ本体部分はUTF-8を採用する。
例: `012029729820` $\rightarrow$ `"ab"` を出力

## Hello, world!
```cless
012027231013108310831112442323119311131143108310023320
```

コンパイル
```
$ clessc hello.cless
```

実行
```
$ ./cless.out
Hello, world!
```

## Debianパッケージの署名確認

CharlessのDebianパッケージ（`.deb`ファイル）はGPG署名されています。ダウンロードしたパッケージの署名を確認するには、以下の手順を実行してください。

### 1. 公開鍵のインポート

リポジトリに含まれている公開鍵ファイルをインポートします：

```bash
# 公開鍵をインポート
gpg --import gpg-public-key.asc

# または、GitHubから直接インポート
curl -s https://raw.githubusercontent.com/Saku0512/Charless/main/gpg-public-key.asc | gpg --import
```

### 2. 公開鍵の信頼設定（オプション）

```bash
# キーIDを確認
gpg --list-keys --keyid-format LONG | grep "Charless GPG Key"

# キーに署名して信頼する（推奨）
gpg --edit-key B9B12A48239B3B49
# コマンドプロンプトで "trust" と入力し、信頼レベルを選択
```

### 3. パッケージの署名確認

#### `.changes`ファイルから確認（推奨）

```bash
# .changesファイルの署名を確認
dpkg-sig --verify charless_*.changes

# または、GPGで直接確認
gpg --verify charless_*.changes
```

#### `.deb`ファイルから確認

```bash
# .debファイルの署名を確認
dpkg-sig --verify charless_*.deb

# または、debsigsで確認（debsigsパッケージが必要）
debsigs --verify charless_*.deb
```

### 4. 署名確認の結果

正常な場合、以下のようなメッセージが表示されます：

```
Good signature from "Saku0512 (Charless GPG Key) <saku0512sec@gmail.com>"
```

### GPGキー情報

- **キーID**: `B9B12A48239B3B49`
- **フルキーID**: `9D4CA3816FE631A99BF69E0BB9B12A48239B3B49`
- **メールアドレス**: saku0512sec@gmail.com
- **公開鍵ファイル**: `gpg-public-key.asc`
