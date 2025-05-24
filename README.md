# Minishell

42school の minishell プロジェクト - シンプルなシェルの実装

## 概要

このプロジェクトは、bash の基本機能を模倣したシンプルなシェルを実装します。

## 機能

### 必須機能

- インタラクティブモード
- コマンド履歴
- 実行ファイルの検索と実行（PATH、相対パス、絶対パス）
- クォート処理（シングル・ダブルクォート）
- リダイレクション（`<`, `>`, `>>`, `<<`）
- パイプ（`|`）
- 環境変数（`$VAR`, `$?`）
- シグナル処理（Ctrl-C, Ctrl-D, Ctrl-\）

### ビルトインコマンド

- `echo` (-n オプション付き)
- `cd` (相対パス・絶対パス)
- `pwd`
- `export`
- `unset`
- `env`
- `exit`

### ボーナス機能

- 論理演算子（`&&`, `||`）
- ワイルドカード（`*`）

## コンパイル

```bash
make
```

ボーナス機能付きでコンパイル：

```bash
make bonus
```

## 使用方法

```bash
./minishell
```

## テスト

基本機能のテスト：

```bash
./test_minishell.sh
```

## プロジェクト構造

```
minishell/
├── includes/
│   └── minishell.h
├── src/
│   ├── main.c
│   ├── lexer/
│   ├── parser/
│   ├── executor/
│   ├── utils/
│   └── bonus/
├── libft/
├── Makefile
└── README.md
```

## 作成者

kizuna - 42Tokyo
