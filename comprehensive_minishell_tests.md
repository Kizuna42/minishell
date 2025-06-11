# 42 Minishell 包括的評価テスト

## 1. Compile

### 1.1 コンパイルフラグ確認

```bash
# テスト: makefileのコンパイルフラグ確認
make -n
# 期待: -Wall -Wextra -Werror が含まれている

# テスト: コンパイル実行
make
# 期待: エラーなくコンパイル完了

# テスト: 再リンク確認
make
# 期待: "Nothing to be done" または再リンクしない
```

## 2. Simple Command & Global Variables

### 2.1 絶対パスコマンド実行

```bash
# テスト: 絶対パスでのコマンド実行
/bin/ls
# 期待: カレントディレクトリの内容表示

/bin/pwd
# 期待: カレントディレクトリのパス表示

/bin/echo hello
# 期待: hello
```

### 2.2 グローバル変数確認

```bash
# 質問: グローバル変数はいくつ使用？
# 答え: 1つ (g_signal_status)
# 理由: シグナル処理のためのみに使用、他のデータ構造へのアクセスは提供しない
```

### 2.3 空コマンドテスト

```bash
# テスト: 空コマンド

# 期待: 新しいプロンプト表示

# テスト: スペースのみ

# 期待: 新しいプロンプト表示

# テスト: タブのみ

# 期待: 新しいプロンプト表示
```

## 3. Arguments & History

### 3.1 引数付きコマンド

```bash
# テスト: 引数付きコマンド
/bin/ls -l
# 期待: 詳細リスト表示

/bin/echo hello world
# 期待: hello world

/bin/ls -la /tmp
# 期待: /tmpの詳細リスト表示

# テスト: 履歴機能 (↑キーで前のコマンド)
# 期待: 前のコマンドが再表示される
```

## 4. Echo

### 4.1 echo 基本テスト

```bash
# テスト: echo基本
echo hello
# 期待: hello

echo hello world
# 期待: hello world

echo
# 期待: (空行)

# テスト: -nオプション
echo -n hello
# 期待: hello (改行なし)

echo -n -n -n hello
# 期待: hello (改行なし)

echo -nnn hello
# 期待: hello (改行なし)
```

## 5. Exit

### 5.1 exit 基本テスト

```bash
# テスト: exit引数なし
exit
# 期待: シェル終了、終了コード0

# テスト: exit数値
exit 42
# 期待: シェル終了、終了コード42

# テスト: exit負数
exit -1
# 期待: シェル終了、終了コード255

# テスト: exit無効引数
exit abc
# 期待: エラーメッセージ、終了コード255

# テスト: exit複数引数
exit 1 2
# 期待: エラーメッセージ、継続実行、終了コード1
```

## 6. Return Value of Process

### 6.1 終了ステータステスト

```bash
# テスト: 成功コマンド
/bin/ls
echo $?
# 期待: 0

# テスト: 失敗コマンド
/bin/ls nonexistent
echo $?
# 期待: 2

# テスト: 存在しないコマンド
nonexistentcommand
echo $?
# 期待: 127

# テスト: ディレクトリ実行
./
echo $?
# 期待: 126
```

## 7. Signals

### 7.1 空プロンプトでのシグナル

```bash
# テスト: Ctrl-C (空プロンプト)
# 操作: Ctrl-C
# 期待: 新しい行に新しいプロンプト表示

# テスト: Ctrl-\ (空プロンプト)
# 操作: Ctrl-\
# 期待: 何も起こらない

# テスト: Ctrl-D (空プロンプト)
# 操作: Ctrl-D
# 期待: minishell終了
```

### 7.2 入力中のシグナル

```bash
# テスト: 入力中のCtrl-C
# 操作: "hello" と入力してCtrl-C
# 期待: 新しい行に新しいプロンプト、入力内容クリア

# テスト: 入力中のCtrl-D
# 操作: "hello" と入力してCtrl-D
# 期待: 何も起こらない

# テスト: 入力中のCtrl-\
# 操作: "hello" と入力してCtrl-\
# 期待: 何も起こらない
```

### 7.3 ブロッキングコマンド中のシグナル

```bash
# テスト: cat中のCtrl-C
cat
# Ctrl-C
# 期待: プロセス終了、新しいプロンプト

# テスト: cat中のCtrl-\
cat
# Ctrl-\
# 期待: プロセス終了、"Quit (core dumped)"表示

# テスト: grep中のCtrl-C
grep pattern
# Ctrl-C
# 期待: プロセス終了、新しいプロンプト
```

## 8. Double Quotes

### 8.1 ダブルクォートテスト

```bash
# テスト: 基本ダブルクォート
echo "hello world"
# 期待: hello world

echo "cat lol.c | cat > lol.c"
# 期待: cat lol.c | cat > lol.c

# テスト: 変数展開
echo "Hello $USER"
# 期待: Hello [ユーザー名]

# テスト: 特殊文字
echo "Hello\nworld"
# 期待: Hello\nworld

# テスト: 空文字列
echo ""
# 期待: (空行)
```

## 9. Simple Quotes

### 9.1 シングルクォートテスト

```bash
# テスト: 基本シングルクォート
echo 'hello world'
# 期待: hello world

# テスト: 変数展開なし
echo '$USER'
# 期待: $USER

# テスト: 特殊文字無効化
echo 'cat | grep | >'
# 期待: cat | grep | >

# テスト: 空文字列
echo ''
# 期待: (空行)

# テスト: パイプ無効化
echo 'hello | world'
# 期待: hello | world
```

## 10. env

### 10.1 環境変数表示

```bash
# テスト: env基本
env
# 期待: 全環境変数表示

# テスト: env引数付き
env arg
# 期待: エラーメッセージ、終了コード1

# テスト: 特定変数確認
env | grep USER
# 期待: USER関連変数表示
```

## 11. export

### 11.1 環境変数エクスポート

```bash
# テスト: export表示
export
# 期待: export形式で全変数表示

# テスト: 新規変数作成
export TESTVAR=hello
echo $TESTVAR
# 期待: hello

# テスト: 変数上書き
export USER=testuser
echo $USER
# 期待: testuser

# テスト: 無効な識別子
export 123=value
# 期待: エラーメッセージ、終了コード1

# テスト: 無効なオプション
export --invalid=value
# 期待: エラーメッセージ、終了コード2

export -invalid=value
# 期待: エラーメッセージ、終了コード2

# テスト: 値なし変数
export EMPTYVAR=
echo $EMPTYVAR
# 期待: (空文字)
```

## 12. unset

### 12.1 環境変数削除

```bash
# テスト: 変数削除
export TESTVAR=hello
unset TESTVAR
echo $TESTVAR
# 期待: (空文字)

# テスト: 存在しない変数
unset NONEXISTENT
# 期待: エラーなし、終了コード0

# テスト: 無効な識別子
unset 123
# 期待: エラーメッセージ、終了コード1

# テスト: 無効なオプション
unset -invalid
# 期待: エラーメッセージ、終了コード2

# テスト: 引数なし
unset
# 期待: エラーなし、終了コード0
```

## 13. cd

### 13.1 ディレクトリ変更

```bash
# テスト: cd基本
pwd
cd /tmp
pwd
# 期待: /tmp

# テスト: cd相対パス
cd ..
pwd
# 期待: 上位ディレクトリ

# テスト: cd現在ディレクトリ
cd .
pwd
# 期待: 変更なし

# テスト: cdホーム
cd
pwd
# 期待: ホームディレクトリ

# テスト: cd存在しないディレクトリ
cd nonexistent
# 期待: エラーメッセージ、終了コード1

# テスト: cd -
export OLDPWD=/tmp
cd -
pwd
# 期待: /tmp表示、ディレクトリ変更

# テスト: cd複数引数
cd /tmp extra
# 期待: エラーメッセージ、終了コード1
```

## 14. pwd

### 14.1 現在ディレクトリ表示

```bash
# テスト: pwd基本
pwd
# 期待: 現在のディレクトリパス

# テスト: pwd引数付き
pwd arg
# 期待: エラーメッセージ、終了コード1

# テスト: ディレクトリ変更後
cd /tmp
pwd
# 期待: /tmp
```

## 15. Relative Path

### 15.1 相対パス実行

```bash
# テスト: 相対パスコマンド
./minishell
# 期待: minishell起動（存在する場合）

# テスト: 複雑な相対パス
cd /tmp
../bin/ls
# 期待: エラーまたはlsコマンド実行

# テスト: . と .. の組み合わせ
././../bin/ls
# 期待: エラーまたはlsコマンド実行
```

## 16. Environment Path

### 16.1 PATH 環境変数テスト

```bash
# テスト: PATH使用コマンド
ls
# 期待: カレントディレクトリ内容表示

cat /etc/passwd
# 期待: パスワードファイル表示

# テスト: PATH削除
unset PATH
ls
# 期待: エラーメッセージ、終了コード127

# テスト: PATH復元
export PATH="/bin:/usr/bin"
ls
# 期待: カレントディレクトリ内容表示

# テスト: PATH順序確認
export PATH="/tmp:/bin"
# （/tmpにlsファイルがある場合）
ls
# 期待: /tmp/lsが実行される
```

## 17. Redirection

### 17.1 入力リダイレクション

```bash
# テスト: 入力リダイレクション
echo "hello" > testfile
cat < testfile
# 期待: hello

# テスト: 存在しないファイル
cat < nonexistent
# 期待: エラーメッセージ、終了コード1
```

### 17.2 出力リダイレクション

```bash
# テスト: 出力リダイレクション
echo "hello" > testfile
cat testfile
# 期待: hello

# テスト: 追記リダイレクション
echo "world" >> testfile
cat testfile
# 期待: hello\nworld

# テスト: 複数リダイレクション
echo "test" > file1 > file2
cat file1
cat file2
# 期待: file1は空、file2に"test"
```

### 17.3 ヒアドキュメント

```bash
# テスト: 基本ヒアドキュメント
cat << EOF
line1
line2
EOF
# 期待: line1\nline2

# テスト: 変数展開ヒアドキュメント
export VAR=test
cat << EOF
Hello $VAR
EOF
# 期待: Hello test

# テスト: クォートヒアドキュメント
cat << 'EOF'
Hello $VAR
EOF
# 期待: Hello $VAR
```

## 18. Pipes

### 18.1 基本パイプ

```bash
# テスト: 基本パイプ
echo "hello world" | cat
# 期待: hello world

ls | grep minishell
# 期待: minishell関連ファイル表示

# テスト: 複数パイプ
echo "hello\nworld\nhello" | grep hello | wc -l
# 期待: 2

# テスト: 失敗コマンドとパイプ
ls nonexistent | grep something
# 期待: エラーメッセージ、次のコマンドは実行
```

### 18.2 パイプとリダイレクション混合

```bash
# テスト: パイプとリダイレクション
echo "hello" | cat > output
cat output
# 期待: hello

# テスト: 複雑な組み合わせ
ls | grep -v test > filtered
cat filtered
# 期待: "test"を含まないファイル一覧
```

## 19. Go Crazy and History

### 19.1 エラーハンドリング

```bash
# テスト: 存在しないコマンド
nonexistentcommand
# 期待: エラーメッセージ、シェル継続

# テスト: 長いコマンド
echo very very very very very very very very very very very long command with many arguments here and there and everywhere
# 期待: 正常実行

# テスト: Ctrl-Cとバッファクリア
# 操作: コマンド入力中にCtrl-C、その後Enter
# 期待: 空コマンド実行、何も起こらない
```

### 19.2 履歴ナビゲーション

```bash
# テスト: 履歴機能
echo "first"
echo "second"
# ↑キー2回
# 期待: "echo first"が表示
# ↓キー1回
# 期待: "echo second"が表示
```

## 20. Environment Variables

### 20.1 変数展開

```bash
# テスト: 基本変数展開
echo $USER
# 期待: ユーザー名表示

echo $HOME
# 期待: ホームディレクトリパス

# テスト: 存在しない変数
echo $NONEXISTENT
# 期待: (空文字)

# テスト: ダブルクォート内変数展開
echo "Hello $USER"
# 期待: Hello [ユーザー名]

# テスト: シングルクォート内変数展開
echo 'Hello $USER'
# 期待: Hello $USER

# テスト: 終了ステータス変数
ls
echo $?
# 期待: 0

# テスト: プロセスID変数
echo $$
# 期待: プロセスID数値
```

## 21. Bonus Features

### 21.1 論理演算子 (&&, ||)

```bash
# テスト: && 演算子
true && echo "success"
# 期待: success

false && echo "should not print"
# 期待: (何も表示されない)

# テスト: || 演算子
true || echo "should not print"
# 期待: (何も表示されない)

false || echo "failure"
# 期待: failure

# テスト: 複合論理演算
true && echo "first" || echo "second"
# 期待: first

# テスト: 括弧
(echo "grouped" && echo "commands")
# 期待: grouped\ncommands
```

### 21.2 ワイルドカード

```bash
# テスト: ワイルドカード展開
echo *.c
# 期待: 現在ディレクトリの.cファイル一覧

ls *.h
# 期待: .hファイル一覧

echo test*
# 期待: testで始まるファイル一覧

# テスト: マッチなし
echo *.nonexistent
# 期待: *.nonexistent (そのまま)
```

## 22. Special Tests

### 22.1 エッジケース

```bash
# テスト: 空変数
export EMPTY=""
echo "$EMPTY"
# 期待: (空行)

# テスト: 特殊文字
echo "special chars: | > < & ; () $ * ?"
# 期待: special chars: | > < & ; () $ * ?

# テスト: 長い行
echo "very long line with many many many many many characters to test buffer handling and memory management in the shell implementation"
# 期待: 正常出力

# テスト: ネストしたクォート
echo "outer 'inner' quote"
# 期待: outer 'inner' quote

echo 'outer "inner" quote'
# 期待: outer "inner" quote
```

### 22.2 メモリ・リークテスト

```bash
# テスト: 大量コマンド実行
for i in {1..100}; do echo "test $i"; done
# 期待: test 1 から test 100 まで表示

# テスト: 大量変数作成・削除
for i in {1..50}; do export "VAR$i=value$i"; done
for i in {1..50}; do unset "VAR$i"; done
# 期待: エラーなし、メモリリークなし
```

## テスト実行前の準備

```bash
# テストファイル作成
touch testfile
echo "test content" > testfile

# テスト用ディレクトリ作成
mkdir testdir

# 実行権限のないファイル作成
touch noperm
chmod 000 noperm
```

## テスト実行後のクリーンアップ

```bash
# テストファイル削除
rm -f testfile output filtered file1 file2 noperm
rm -rf testdir

# 環境変数クリーンアップ
unset TESTVAR EMPTYVAR VAR
```

---

**注意事項:**

1. 各テストは別々の minishell セッションで実行することを推奨
2. 期待する結果は bash の動作と一致させる
3. メモリリークは`valgrind`で確認
4. クラッシュやハングした場合は該当フラグを使用
5. 実装されていない機能は適切にエラーを返すこと
