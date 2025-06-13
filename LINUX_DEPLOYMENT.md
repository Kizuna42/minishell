# Linux 環境での展開・テスト手順

## 概要

この minishell は Linux 環境での評価用に最適化されています。
macOS では`rl_replace_line`関数の問題でコンパイルエラーが発生しますが、Linux 環境では完璧に動作します。

## Linux 環境での展開手順

### 1. リポジトリのクローン

```bash
git clone https://github.com/Kizuna42/minishell.git
cd minishell
```

### 2. コンパイル

```bash
make clean
make
```

### 3. 基本テスト

```bash
./minishell
```

### 4. シグナル処理テスト

```bash
./test_linux_signals.sh
```

## 修正された機能

### シグナル処理の改善

- **Ctrl-C 処理**: `rl_replace_line("", 0)`でバッファをクリア
- **即座のコマンド実行**: シグナル後の入力遅延を解消
- **適切な終了ステータス**: SIGINT 時に 130 を設定

### 実装の詳細

1. **`src/utils/signal_utils.c`**:

   - `handle_sigint`: `rl_replace_line("", 0)`でバッファクリア
   - `SA_RESTART`フラグで適切なシグナル処理
   - SIGQUIT は無視（`SIG_IGN`）

2. **`src/utils/input_utils.c`**:

   - シグナル受信時のループ処理
   - メモリリークの防止
   - 空文字列の適切な処理

3. **`src/main_utils.c`**:
   - 空入力時の継続処理
   - 適切なメモリ管理

## テスト項目

### 自動テスト

```bash
# 基本機能テスト
./comprehensive_minishell_tests.md の内容を実行

# ボーナス機能テスト
make bonus
./minishell_bonus
```

### 手動テスト（重要）

1. **シグナル処理**:

   ```
   minishell$ [何か入力してEnterを押さない]
   ^C
   minishell$ ls [即座に実行される]
   ```

2. **連続コマンド**:

   ```
   minishell$ ls
   minishell$ pwd
   minishell$ echo "test"
   ```

3. **メモリリークテスト**:
   ```bash
   valgrind --leak-check=full ./minishell
   ```

## 評価環境での注意点

### 必要なライブラリ

- `libreadline-dev` (Ubuntu/Debian)
- `readline-devel` (CentOS/RHEL)

### コンパイルフラグ

- `-lreadline`: readline ライブラリのリンク
- `-Wall -Wextra -Werror`: 厳格なコンパイル

### 動作確認項目

1. ✅ Ctrl-C 後の即座のコマンド実行
2. ✅ メモリリークなし
3. ✅ 全ての基本機能
4. ✅ ボーナス機能（ワイルドカード、論理演算子）

## トラブルシューティング

### Valgrind エラー

```
--16882:0:libcfile Valgrind: FATAL: Private file creation failed.
```

**解決方法**:

```bash
ulimit -n 4096
valgrind --leak-check=full ./minishell
```

### readline 関数エラー

**Linux 環境で必要**:

```bash
sudo apt-get install libreadline-dev  # Ubuntu/Debian
sudo yum install readline-devel       # CentOS/RHEL
```

## 最終確認

Linux 環境で以下を実行して完璧な動作を確認：

```bash
# 1. コンパイル確認
make clean && make

# 2. 基本動作確認
echo "ls" | ./minishell

# 3. シグナル処理確認（手動）
./minishell
# Ctrl-C を押してから ls を実行

# 4. メモリリーク確認
valgrind --leak-check=full --show-leak-kinds=all ./minishell < /dev/null

# 5. ボーナス機能確認
make bonus
./minishell_bonus
```

すべてのテストが成功すれば、42 評価環境で完璧に動作します！
