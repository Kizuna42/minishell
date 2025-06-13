# 42 Minishell 評価基準テスト

## 評価フラグ

- **Crash**: segfault, bus error, double free 等でクラッシュ
- **Incomplete work**: 機能が動作しない、bash と動作が異なる
- **Leaks**: メモリリーク検出
- **Norme**: norminette 違反

---

## 1. Compile

### 1.1 コンパイル必須チェック ⚠️

```bash
# 評価基準: コンパイルフラグ確認
make -n
# 期待: -Wall -Wextra -Werror が含まれている
# 減点要素: 指定フラグがない → invalid compilation flags

# 評価基準: エラーなしコンパイル
make
# 期待: エラーなくコンパイル完了
# 減点要素: コンパイルエラー → invalid compilation

# 評価基準: 再リンク防止
make
# 期待: "Nothing to be done" または再リンクしない
# 減点要素: 不必要な再リンク → incomplete work

# 評価基準: norminette準拠
norminette src/ includes/
# 期待: エラーなし
# 減点要素: norm違反 → norme flag
```

## 2. Simple Command & Global Variables

### 2.1 基本コマンド実行 ⚠️

```bash
# 評価基準: 絶対パスコマンド実行
/bin/ls
/bin/pwd
/bin/echo hello
# 期待: 正常実行
# 減点要素: 実行不可 → crash or incomplete work

# 評価基準: 空コマンド処理
(空のエンター)
# 期待: 新しいプロンプト表示
# 減点要素: クラッシュ → crash

# 評価基準: 空白のみコマンド


# 期待: 新しいプロンプト表示
# 減点要素: クラッシュ → crash
```

### 2.2 グローバル変数制限 ⚠️

```bash
# 評価基準: グローバル変数の数と理由
# 質問: 「How many global variables? why?」
# 答え: 1つ (シグナル処理用)
# 減点要素: 不適切な数や理由 → incomplete work
```

### 2.3 空コマンドテスト

```bash
# テスト: 空コマンド
(空のエンター)
# 期待: 新しいプロンプト表示

# テスト: スペースのみ

# 期待: 新しいプロンプト表示

# テスト: タブのみ

# 期待: 新しいプロンプト表示
```

## 3. Arguments & History

### 3.1 引数付きコマンド ⚠️

```bash
# 評価基準: 引数付き絶対パスコマンド
/bin/ls -l
/bin/echo hello world
/bin/ls -la /tmp
# 期待: 正常実行
# 減点要素: 引数処理不可 → incomplete work

# 評価基準: 履歴機能
echo "test1"
echo "test2"
# ↑キーで履歴確認
# 期待: 前のコマンドが表示される
# 減点要素: 履歴が動作しない → incomplete work
```

## 4. Echo

### 4.1 echo 実装 ⚠️

```bash
# 評価基準: echo基本動作
echo hello
echo hello world
echo
# 期待: 正常出力
# 減点要素: 動作不良 → incomplete work

# 評価基準: -nオプション
echo -n hello
echo -n -n -n hello
echo -nnn hello
# 期待: 改行なし出力
# 減点要素: -n処理不正 → incomplete work
```

## 5. Exit

### 5.1 exit 実装 ⚠️

```bash
# 評価基準: exit基本動作
exit
# 期待: シェル終了、終了コード0

exit 42
# 期待: シェル終了、終了コード42

# 評価基準: exit無効引数
exit abc
# 期待: エラーメッセージ、シェル終了、終了コード255

# 評価基準: exit複数引数
exit 1 2
# 期待: "too many arguments"エラー、継続実行、終了コード1
# 減点要素: 処理不正 → incomplete work
```

## 6. Return Value of Process ⚠️

### 6.1 終了ステータス ($?) 正確性

```bash
# 評価基準: 成功コマンドの終了コード
/bin/ls
echo $?
# 期待: 0
# bashと比較して確認

# 評価基準: 失敗コマンドの終了コード
/bin/ls filethatdoesntexist
echo $?
# 期待: 2
# bashと比較して確認

# 評価基準: 存在しないコマンド
dsbksdgbksdghsd
echo $?
# 期待: 127
# 減点要素: bashと異なる終了コード → incomplete work
```

## 7. Signals ⚠️

### 7.1 シグナル処理の正確性

```bash
# 評価基準: 空プロンプトでのCtrl-C
# 操作: Ctrl-C
# 期待: 新しい行に新しいプロンプト表示
# 減点要素: 動作不良 → incomplete work

# 評価基準: 空プロンプトでのCtrl-\
# 操作: Ctrl-\
# 期待: 何も起こらない
# 減点要素: 終了やクラッシュ → incomplete work

# 評価基準: 空プロンプトでのCtrl-D
# 操作: Ctrl-D
# 期待: minishell終了
# 減点要素: 動作不良 → incomplete work

# 評価基準: 入力中のCtrl-C
# 操作: "hello"入力後Ctrl-C
# 期待: 新しい行に新しいプロンプト、バッファクリア
# 減点要素: バッファが残る → incomplete work

# 評価基準: 入力中のCtrl-D
# 操作: "hello"入力後Ctrl-D
# 期待: 何も起こらない
# 減点要素: 予期しない動作 → incomplete work

# 評価基準: 入力中のCtrl-\
# 操作: "hello"入力後Ctrl-\
# 期待: 何も起こらない
# 減点要素: 終了やクラッシュ → incomplete work

# 評価基準: ブロッキングコマンド中のシグナル
cat
# Ctrl-C
# 期待: プロセス終了、新しいプロンプト

cat
# Ctrl-\
# 期待: プロセス終了、"Quit (core dumped)"表示

# 減点要素: シグナル処理不備 → incomplete work or crash
```

## 8. Double Quotes ⚠️

### 8.1 ダブルクォート処理

```bash
# 評価基準: ダブルクォート基本処理
echo "cat lol.c | cat > lol.c"
# 期待: cat lol.c | cat > lol.c

# 評価基準: 変数展開
echo "Hello $USER"
# 期待: Hello [ユーザー名]

# 評価基準: 空文字列
echo ""
# 期待: 空行
# 減点要素: 処理不正 → incomplete work
```

## 9. Simple Quotes ⚠️

### 9.1 シングルクォート処理

```bash
# 評価基準: 変数展開無効化
echo '$USER'
# 期待: $USER (展開されない)

# 評価基準: 特殊文字無効化
echo 'cat | grep | >'
# 期待: cat | grep | >

# 評価基準: 空引数
echo ''
# 期待: 空行
# 減点要素: Nothing should be interpreted → incomplete work
```

## 10. env ⚠️

### 10.1 環境変数表示

```bash
# 評価基準: 現在の環境変数表示
env
# 期待: 全環境変数表示
# 減点要素: 表示されない → incomplete work
```

## 11. export ⚠️

### 11.1 環境変数エクスポート

```bash
# 評価基準: 新規変数作成
export TESTVAR=hello
env | grep TESTVAR
# 期待: TESTVAR=hello が表示される

# 評価基準: 既存変数置換
export USER=testuser
echo $USER
# 期待: testuser
# 減点要素: 変数設定不可 → incomplete work
```

## 12. unset ⚠️

### 12.1 環境変数削除

```bash
# 評価基準: 変数削除
export TESTVAR=hello
unset TESTVAR
env | grep TESTVAR
# 期待: 何も表示されない（削除済み）
# 減点要素: 削除不可 → incomplete work
```

## 13. cd ⚠️

### 13.1 ディレクトリ変更

```bash
# 評価基準: 基本的なcd動作
cd /tmp
/bin/pwd
# 期待: /tmp

# 評価基準: 相対パス
cd ..
/bin/pwd
# 期待: 上位ディレクトリ

# 評価基準: . と ..
cd .
cd ..
/bin/pwd
# 期待: 正常なディレクトリ変更

# 評価基準: 存在しないディレクトリ
cd nonexistentdirectory
# 期待: エラーメッセージ、継続実行
# 減点要素: クラッシュ → crash
```

## 14. pwd ⚠️

### 14.1 現在ディレクトリ表示

```bash
# 評価基準: pwd基本動作
pwd
# 期待: 現在のディレクトリパス表示

# 評価基準: 複数ディレクトリでのテスト
cd /tmp
pwd
cd /usr
pwd
# 期待: 各ディレクトリパスの正確な表示
# 減点要素: 不正確なパス → incomplete work
```

## 15. Relative Path ⚠️

### 15.1 相対パス実行

```bash
# 評価基準: 相対パスコマンド実行
./minishell  # 存在する場合
# 期待: 実行される

# 評価基準: 複雑な相対パス
cd /tmp
ls ../../bin/ls
# 期待: 正常実行またはエラー（パスに依存）
# 減点要素: 相対パス処理不可 → incomplete work
```

## 16. Environment Path ⚠️

### 16.1 PATH 環境変数

```bash
# 評価基準: PATH使用コマンド
ls
# 期待: 実行される

# 評価基準: PATH削除後
unset PATH
ls
# 期待: "command not found"エラー

# 評価基準: PATH複数ディレクトリ
export PATH="/bin:/usr/bin"
ls
# 期待: 左から右の順序で検索、実行
# 減点要素: PATH処理不可 → incomplete work
```

## 17. Redirection ⚠️

### 17.1 リダイレクション処理

```bash
# 評価基準: 入力リダイレクション
echo "hello" > testfile
cat < testfile
# 期待: hello

# 評価基準: 出力リダイレクション
echo "hello" > output
cat output
# 期待: hello

# 評価基準: 追記リダイレクション
echo "world" >> output
cat output
# 期待: hello\nworld

# 評価基準: ヒアドキュメント
cat << EOF
line1
line2
EOF
# 期待: line1\nline2

# 評価基準: 複数リダイレクション失敗テスト
echo hello > > file
# 期待: シンタックスエラー
# 減点要素: リダイレクション処理不可 → incomplete work
```

## 18. Pipes ⚠️

### 18.1 パイプ処理

```bash
# 評価基準: 基本パイプ
cat testfile | grep hello
# 期待: 正常なパイプ処理

# 評価基準: 複数パイプ
cat testfile | grep bla | more
# 期待: 連続パイプ処理

# 評価基準: 失敗コマンドとパイプ
ls filethatdoesntexist | grep bla | more
# 期待: エラー表示、後続コマンド実行

# 評価基準: パイプとリダイレクション混合
cat testfile | grep hello > output
# 期待: 正常処理
# 減点要素: パイプ処理不可 → incomplete work or crash
```

## 19. Go Crazy and History ⚠️

### 19.1 エラーハンドリング

```bash
# 評価基準: Ctrl-C後のバッファクリア
# 操作: コマンド入力中にCtrl-C、その後Enter
# 期待: 空コマンド実行、何も起こらない
# 減点要素: バッファが残る → incomplete work

# 評価基準: 存在しないコマンド
dsbksdgbksdghsd
# 期待: エラーメッセージ、シェル継続
# 減点要素: クラッシュ → crash

# 評価基準: 長いコマンド
echo very very very very very very long command with many arguments
# 期待: 正常実行
# 減点要素: クラッシュ → crash

# 評価基準: 履歴ナビゲーション
echo "test1"
echo "test2"
# ↑↓キーでナビゲーション
# 期待: 正確な履歴表示
# 減点要素: 履歴機能不良 → incomplete work
```

## 20. Environment Variables ⚠️

### 20.1 変数展開

```bash
# 評価基準: 基本変数展開
echo $USER
# 期待: ユーザー名表示

# 評価基準: ダブルクォート内変数展開
echo "$USER"
# 期待: ユーザー名表示

# 評価基準: $USER存在確認またはセット
export USER=testuser
echo $USER
# 期待: testuser

echo "$USER"
# 期待: testuser
# 減点要素: 変数展開不可 → incomplete work
```

## 21. Bonus (mandatory 完璧時のみ評価) ⚠️

### 21.1 論理演算子

```bash
# 評価基準: && 演算子
true && echo "success"
# 期待: success

false && echo "should not print"
# 期待: 何も表示されない

# 評価基準: || 演算子
true || echo "should not print"
# 期待: 何も表示されない

false || echo "failure"
# 期待: failure

# 評価基準: 括弧
(echo "grouped" && echo "commands")
# 期待: grouped\ncommands
```

### 21.2 ワイルドカード

```bash
# 評価基準: ワイルドカード展開
echo *.c
# 期待: .cファイル一覧（ない場合は *.c そのまま）

echo *.h
# 期待: .hファイル一覧（ない場合は *.h そのまま）
```

### 21.3 サプライズテスト

```bash
# 評価基準: 特定の変数テスト
export USER=testvalue
echo "$USER"
# 期待: testvalue

echo '$USER'
# 期待: $USER
# 減点要素: ボーナス機能不良 → bonus評価なし
```

## メモリリーク検出 ⚠️

```bash
# 評価基準: valgrindによるメモリリーク検出
valgrind --leak-check=full --show-leak-kinds=all ./minishell
# 各種コマンド実行後exit
# 期待: "All heap blocks were freed -- no leaks are possible"
# 減点要素: メモリリーク検出 → leaks flag
```

## 評価チェックリスト

### ✅ 必須確認項目

- [ ] コンパイル: フラグ確認、エラーなし、再リンクなし
- [ ] グローバル変数: 適切な数と理由
- [ ] シグナル: Ctrl-C/\\\\/D の全場面での正確な動作
- [ ] ビルトイン: echo, exit, env, export, unset, cd, pwd
- [ ] リダイレクション: <, >, >>, <<
- [ ] パイプ: 基本、複数、失敗コマンドとの組み合わせ
- [ ] 変数展開: $変数、ダブル/シングルクォート
- [ ] エラーハンドリング: クラッシュなし、適切なエラーメッセージ

### ⚠️ 減点回避

- [ ] **Crash**: segfault, bus error, double free なし
- [ ] **Leaks**: valgrind でメモリリークなし
- [ ] **Norme**: norminette 準拠
- [ ] **Incomplete work**: 全機能が bash と同様に動作

### 🎯 高評価ポイント

- [ ] bash との完全互換性（終了コードまで一致）
- [ ] エッジケースの適切な処理
- [ ] ボーナス機能の完全実装（mandatory 完璧時のみ）

---

**注意**: このテストは evaluation.txt の評価基準に基づいています。各項目で動作不良やクラッシュが発生した場合、対応するフラグが設定されます。
