# 🔬 Minishell モジュール詳細分析

## 🔤 Lexer（字句解析）モジュール - `src/lexer/`

### 📋 概要

入力文字列を意味のある単位（トークン）に分割する処理を担当。コンパイラの字句解析器と同じ役割。

### 📁 ファイル構成と責任

#### `lexer.c` - メイン字句解析エンジン

```c
// 主要関数
t_token *tokenize(char *input);           // 入力文字列をトークン列に変換
void process_token(char *input, int *i, t_token **tokens, int advance);
```

**処理フロー:**

```
"echo 'hello world' | grep test"
    ↓
1. 文字を1つずつ走査
2. 空白でトークン境界を判定
3. 特殊文字（|, >, <, etc.）を演算子として認識
4. クォート内は1つのトークンとして処理
    ↓
[TOKEN_WORD:"echo"] [TOKEN_WORD:"hello world"] [TOKEN_PIPE] [TOKEN_WORD:"grep"] [TOKEN_WORD:"test"]
```

#### `lexer_utils.c` - 字句解析補助関数

```c
// 文字種判定
int is_operator(char c);                  // 演算子文字かチェック
int is_quote(char c);                     // クォート文字かチェック
int is_whitespace(char c);                // 空白文字かチェック

// トークン抽出
char *extract_word(char *str, int *i);    // 単語の抽出
t_token_type get_operator_type(char *str, int *advance); // 演算子の種類判定
```

#### `lexer_quote.c` - クォート処理専門

```c
char *extract_quoted_string(char *str, int *i); // クォート内文字列の抽出
```

**クォート処理の複雑さ:**

```bash
# シングルクォート: 全て文字通り
echo 'hello $USER world'  → "hello $USER world"

# ダブルクォート: 変数展開あり
echo "hello $USER world"  → "hello kizuna world"

# エスケープ処理
echo "hello \"world\""    → "hello "world""
```

#### `tokenizer.c` - トークン管理

```c
// トークン操作
t_token *create_token(t_token_type type, char *value);
void add_token(t_token **head, t_token *new_token);
void free_tokens(t_token *tokens);
```

### 🎯 設計のポイント

1. **状態管理**: クォート内外の状態を適切に管理
2. **エラー処理**: 不正なクォートや文法エラーの検出
3. **メモリ効率**: 必要最小限のメモリ使用

---

## 🌳 Parser（構文解析）モジュール - `src/parser/`

### 📋 概要

トークン列を抽象構文木（AST）に変換。プログラムの構造を木構造で表現。

### 🏗️ AST 構造の例

```
"ls -la | grep .c > output.txt"

AST:
    REDIRECT_NODE (>)
    ├── left: PIPE_NODE
    │   ├── left: COMMAND_NODE
    │   │   └── args: ["ls", "-la"]
    │   └── right: COMMAND_NODE
    │       └── args: ["grep", ".c"]
    └── filename: "output.txt"
```

### 📁 ファイル構成と責任

#### `parser.c` - メインパーサー

```c
// 主要関数
t_ast_node *parse(t_token *tokens);                    // トークン列をASTに変換
t_ast_node *create_ast_node(t_node_type type);         // ASTノード作成
void free_ast(t_ast_node *ast);                        // AST解放
```

**再帰下降パーサーの実装:**

```c
// 優先度順の解析
parse()
├── parse_logical_ops()     // && || ; (最低優先度)
    ├── parse_pipeline()    // |
        ├── parse_command() // コマンド + リダイレクト
```

#### `parser_command.c` - コマンド解析

```c
t_ast_node *parse_command(t_token **tokens);
int count_args(t_token *tokens);
```

#### `parser_logical.c` - 論理演算子解析

```c
t_ast_node *parse_logical_ops(t_token **tokens);
```

**論理演算子の優先度:**

```bash
# セミコロン（;）: 最低優先度
cmd1 ; cmd2 && cmd3

# AND（&&）、OR（||）: 同じ優先度、左結合
cmd1 && cmd2 || cmd3  # ((cmd1 && cmd2) || cmd3)
```

#### `parser_redirect.c` - リダイレクト解析

```c
t_ast_node *parse_redirections(t_token **tokens, t_ast_node *cmd);
t_ast_node *create_redirect_node(t_token **tokens);
```

**リダイレクトの種類:**

```bash
cmd > file      # 出力リダイレクト
cmd >> file     # 追記リダイレクト
cmd < file      # 入力リダイレクト
cmd << EOF      # ヒアドキュメント
```

#### `parser_syntax.c` - 構文検証

```c
int validate_syntax(t_token *tokens);
void print_syntax_error(char *token);
```

**検証項目:**

- 演算子の連続（`| |`, `> >`など）
- 不完全なリダイレクト（`>` の後にファイル名なし）
- 括弧の対応（ボーナス機能）

### 🎯 設計のポイント

1. **再帰下降**: 文法規則を関数で表現
2. **優先度処理**: 演算子の優先度を正しく処理
3. **エラー回復**: 構文エラー時の適切な処理

---

## ⚡ Executor（実行エンジン）モジュール - `src/executor/`

### 📋 概要

AST を走査してコマンドを実際に実行。プロセス管理と I/O リダイレクトを担当。

### 🔄 実行戦略

#### ビルトインコマンド vs 外部コマンド

```c
if (is_builtin(command)) {
    // 同一プロセス内で実行
    execute_builtin(args, shell);
} else {
    // 新しいプロセスで実行
    pid = fork();
    if (pid == 0) {
        execve(path, args, envp);
    }
    waitpid(pid, &status, 0);
}
```

### 📁 ファイル構成と責任

#### `executor.c` - メイン実行エンジン

```c
int execute_ast(t_ast_node *ast, t_minishell *shell);
int execute_command(t_ast_node *node, t_minishell *shell);
```

**AST 走査パターン:**

```c
switch (ast->type) {
    case NODE_COMMAND:
        return execute_command(ast, shell);
    case NODE_PIPE:
        return execute_pipeline(ast, shell);
    case NODE_REDIRECT_OUT:
        return execute_redirections(ast, shell);
    // ...
}
```

#### `builtin_commands.c` - 基本ビルトイン

```c
int builtin_echo(char **args);      // echo [-n] [string ...]
int builtin_cd(char **args, t_minishell *shell);        // cd [dir]
int builtin_pwd(void);              // pwd
int builtin_env(char **args, t_minishell *shell);       // env
```

#### `builtin_extra.c` - 高度なビルトイン

```c
int builtin_export(char **args, t_minishell *shell);    // export [var=value]
int builtin_unset(char **args, t_minishell *shell);     // unset [var]
int builtin_exit(char **args, t_minishell *shell);      // exit [code]
```

#### `pipes.c` - パイプライン実行

```c
int execute_pipeline(t_ast_node *node, t_minishell *shell);
```

**パイプ実装の核心:**

```c
// パイプ作成
pipe(pipefd);

// 左側コマンド（書き込み側）
if (fork() == 0) {
    dup2(pipefd[1], STDOUT_FILENO);  // 標準出力をパイプに
    close(pipefd[0]);
    close(pipefd[1]);
    execute_ast(node->left, shell);
}

// 右側コマンド（読み込み側）
if (fork() == 0) {
    dup2(pipefd[0], STDIN_FILENO);   // 標準入力をパイプから
    close(pipefd[0]);
    close(pipefd[1]);
    execute_ast(node->right, shell);
}

// 親プロセス
close(pipefd[0]);
close(pipefd[1]);
wait_for_children();
```

#### `redirections.c` - リダイレクト処理

```c
int execute_redirections(t_ast_node *node, t_minishell *shell);
int handle_input_redirect(char *filename);
int handle_output_redirect(char *filename, int append);
```

#### `heredoc_utils.c` - ヒアドキュメント

```c
int handle_heredoc(char *delimiter, t_minishell *shell);
```

**ヒアドキュメントの実装:**

```c
// パイプを作成して一時的なデータ保存
pipe(pipefd);

// 入力を読み取ってパイプに書き込み
while ((line = readline("> ")) != NULL) {
    if (strcmp(line, delimiter) == 0)
        break;
    write(pipefd[1], line, strlen(line));
    write(pipefd[1], "\n", 1);
}

// 標準入力をパイプの読み込み側に変更
dup2(pipefd[0], STDIN_FILENO);
```

### 🎯 設計のポイント

1. **プロセス管理**: fork/exec/wait の適切な使用
2. **ファイル記述子**: dup2 による I/O リダイレクト
3. **エラー処理**: システムコールの失敗処理
4. **リソース管理**: ファイル記述子のリーク防止

---

## 🛠️ Utils（ユーティリティ）モジュール - `src/utils/`

### 📋 概要

環境変数管理、変数展開、エラー処理など、シェルの基盤機能を提供。

### 📁 主要カテゴリ

#### 🌍 環境変数管理 (`env_*.c`)

```c
// 環境変数操作
t_env *init_env(char **envp);                    // 環境変数リスト初期化
char *get_env_value(char *key, t_minishell *shell);      // 値取得
int set_env_value(char *key, char *value, t_minishell *shell); // 値設定
char **env_to_array(t_minishell *shell);                 // 配列変換
```

#### 🔄 変数展開 (`expand_*.c`)

```c
char *expand_variables(char *str, t_minishell *shell);   // $VAR展開
char *expand_tilde(char *str, t_minishell *shell);       // ~展開
```

**変数展開の複雑さ:**

```bash
echo "$USER's home is $HOME"     # 変数展開
echo '$USER's home is $HOME'     # 展開なし（シングルクォート）
echo "Exit code: $?"             # 特殊変数
echo "Process ID: $$"            # プロセスID
```

#### 🌟 ワイルドカード (`wildcard*.c`)

```c
char **expand_wildcard(char *pattern);          // *パターン展開
```

**ワイルドカード展開:**

```bash
ls *.c          # test.c main.c parser.c
echo test_*     # test_file test_dir
```

#### 🚨 エラー処理 (`error_utils.c`)

```c
void print_error(char *cmd, char *msg);         // エラーメッセージ出力
void perror_exit(char *msg);                    // エラー終了
```

### 🎯 設計のポイント

1. **モジュール性**: 機能ごとの明確な分離
2. **再利用性**: 他のモジュールから利用しやすい設計
3. **堅牢性**: エラー処理の充実

---

## ⭐ Bonus（ボーナス機能）モジュール - `src/bonus/`

### 📋 概要

基本機能を超えた高度な機能を実装。

#### `logical_ops.c` - 論理演算子

```bash
cmd1 && cmd2    # cmd1が成功した場合のみcmd2実行
cmd1 || cmd2    # cmd1が失敗した場合のみcmd2実行
cmd1 ; cmd2     # cmd1の結果に関係なくcmd2実行
```

#### `wildcards.c` - ワイルドカード拡張

```bash
ls test_*.{c,h}     # 複雑なパターンマッチング
echo {1..10}        # 範囲展開
```

---

## 🔗 モジュール間の依存関係

```
main.c
├── lexer/          (字句解析)
├── parser/         (構文解析)
├── executor/       (実行)
│   ├── utils/      (ユーティリティ)
│   └── bonus/      (ボーナス機能)
└── utils/          (共通ユーティリティ)
```

## 🎯 学習の進め方

1. **main.c → main_utils.c**: 全体の流れを理解
2. **lexer/**: 文字列処理とトークン化
3. **parser/**: AST 構築と文法処理
4. **executor/**: プロセス管理と I/O
5. **utils/**: 環境変数と変数展開
6. **bonus/**: 高度な機能

各モジュールは独立性が高いため、興味のある部分から深掘りできます！
