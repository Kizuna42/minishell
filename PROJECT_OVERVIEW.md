# 🔍 Minishell プロジェクト全体像解説

## 📊 プロジェクト構造の鳥瞰図

```
minishell/
├── 📁 includes/           # ヘッダーファイル
│   └── minishell.h       # 全体の構造体・関数宣言
├── 📁 libft/             # 自作ライブラリ（基本的なC関数群）
├── 📁 src/               # メインソースコード
│   ├── main.c            # 🚀 エントリーポイント
│   ├── main_utils.c      # メインループ・初期化
│   ├── 📁 lexer/         # 🔤 字句解析（文字列→トークン）
│   ├── 📁 parser/        # 🌳 構文解析（トークン→AST）
│   ├── 📁 executor/      # ⚡ 実行エンジン（AST→実行）
│   ├── 📁 utils/         # 🛠️ ユーティリティ関数群
│   └── 📁 bonus/         # ⭐ ボーナス機能
└── 📁 src_explained/     # 📚 解説付きソースコード
```

## 🔄 プログラム実行フロー

### 1. 起動フェーズ (main.c)

```
main()
├── init_minishell()      # シェル初期化
├── setup_signal_handlers() # シグナル設定
├── handle_input_loop()   # メインループ開始
└── cleanup_minishell()   # 終了処理
```

### 2. メインループ (main_utils.c)

```
handle_input_loop()
├── readline(PROMPT)      # ユーザー入力取得
├── process_input()       # 入力処理
│   ├── tokenize()        # 字句解析
│   ├── validate_syntax() # 構文チェック
│   ├── parse()           # AST構築
│   └── execute_ast()     # 実行
└── [ループ継続]
```

### 3. 処理パイプライン

```
"ls -la | grep .c"
    ↓ 字句解析 (lexer/)
[TOKEN_WORD:"ls"] [TOKEN_WORD:"-la"] [TOKEN_PIPE] [TOKEN_WORD:"grep"] [TOKEN_WORD:".c"]
    ↓ 構文解析 (parser/)
    AST: PIPE_NODE
         ├── COMMAND_NODE("ls", ["-la"])
         └── COMMAND_NODE("grep", [".c"])
    ↓ 実行 (executor/)
    fork() → execve("ls") | fork() → execve("grep")
```

## 📂 モジュール別機能分類

### 🔤 Lexer（字句解析）- `src/lexer/`

**責任**: 入力文字列をトークンに分割

| ファイル        | 機能                        |
| --------------- | --------------------------- |
| `lexer.c`       | メイン字句解析ロジック      |
| `lexer_utils.c` | 字句解析補助関数            |
| `lexer_quote.c` | クォート処理（'...' "..."） |
| `tokenizer.c`   | トークン生成・管理          |

**処理例**:

```c
"echo 'hello world'" → [TOKEN_WORD:"echo"] [TOKEN_WORD:"hello world"]
"ls | grep .c"       → [TOKEN_WORD:"ls"] [TOKEN_PIPE] [TOKEN_WORD:"grep"] [TOKEN_WORD:".c"]
```

### 🌳 Parser（構文解析）- `src/parser/`

**責任**: トークン列から抽象構文木(AST)を構築

| ファイル            | 機能                         |
| ------------------- | ---------------------------- |
| `parser.c`          | メインパーサー・AST 構築     |
| `parser_command.c`  | コマンド解析                 |
| `parser_logical.c`  | 論理演算子解析（&&, \|\|）   |
| `parser_redirect.c` | リダイレクト解析（>, <, >>） |
| `parser_syntax.c`   | 構文エラー検出               |
| `parser_*.c`        | その他パーサー補助機能       |

**AST 構造例**:

```
"ls | grep .c > output.txt"
    REDIRECT_NODE (>)
    ├── PIPE_NODE
    │   ├── COMMAND_NODE("ls")
    │   └── COMMAND_NODE("grep", [".c"])
    └── filename: "output.txt"
```

### ⚡ Executor（実行エンジン）- `src/executor/`

**責任**: AST を走査してコマンドを実行

| ファイル             | 機能                   |
| -------------------- | ---------------------- |
| `executor.c`         | メイン実行ロジック     |
| `builtin.c`          | ビルトインコマンド判定 |
| `builtin_commands.c` | echo, cd, pwd, env     |
| `builtin_extra.c`    | export, unset, exit    |
| `pipes.c`            | パイプライン実行       |
| `redirections.c`     | リダイレクト処理       |
| `heredoc_utils.c`    | ヒアドキュメント（<<） |

**実行戦略**:

- **ビルトイン**: 同一プロセス内で実行
- **外部コマンド**: fork() + execve()
- **パイプ**: pipe() + 複数プロセス
- **リダイレクト**: dup2() でファイル記述子操作

### 🛠️ Utils（ユーティリティ）- `src/utils/`

**責任**: 環境変数、変数展開、エラー処理など

| カテゴリ           | ファイル               | 機能                        |
| ------------------ | ---------------------- | --------------------------- |
| **環境変数**       | `env_*.c`              | 環境変数管理・操作          |
| **変数展開**       | `expand_*.c`           | $VAR, ~, ワイルドカード展開 |
| **ワイルドカード** | `wildcard*.c`          | \* パターンマッチング       |
| **エラー処理**     | `error_utils.c`        | エラーメッセージ・終了処理  |
| **シグナル**       | `signal_utils.c`       | Ctrl+C, Ctrl+D 処理         |
| **その他**         | `utils.c`, `cleanup.c` | 汎用関数・メモリ管理        |

### ⭐ Bonus（ボーナス機能）- `src/bonus/`

**責任**: 追加機能の実装

| ファイル        | 機能                      |
| --------------- | ------------------------- |
| `logical_ops.c` | 論理演算子（&&, \|\|, ;） |
| `wildcards.c`   | ワイルドカード拡張        |

## 🔧 重要なデータ構造

### t_token（トークン）

```c
typedef struct s_token {
    t_token_type    type;        // TOKEN_WORD, TOKEN_PIPE, etc.
    char           *value;       // トークンの文字列値
    int            quote_type;   // クォートの種類
    struct s_token *next;        // 次のトークン
} t_token;
```

### t_ast_node（抽象構文木ノード）

```c
typedef struct s_ast_node {
    t_node_type         type;     // NODE_COMMAND, NODE_PIPE, etc.
    char              **args;     // コマンド引数配列
    char               *filename; // リダイレクト先ファイル
    struct s_ast_node  *left;     // 左の子ノード
    struct s_ast_node  *right;    // 右の子ノード
} t_ast_node;
```

### t_minishell（シェル状態）

```c
typedef struct s_minishell {
    t_env  *env_list;         // 環境変数リスト
    char  **envp;             // 環境変数配列
    int    last_exit_status;  // 最後の終了ステータス
    int    stdin_backup;      // 標準入力のバックアップ
    int    stdout_backup;     // 標準出力のバックアップ
} t_minishell;
```

## 🎯 学習のポイント

### 1. **字句解析の理解**

- 文字列をどうやってトークンに分割するか
- クォート内の特殊文字処理
- 演算子の識別方法

### 2. **構文解析の理解**

- 再帰下降パーサーの実装
- 演算子の優先度処理
- AST 構築のアルゴリズム

### 3. **プロセス管理の理解**

- fork/exec/wait の使い方
- パイプの実装方法
- ファイル記述子の操作

### 4. **メモリ管理の理解**

- 動的メモリの適切な解放
- リークフリーな実装
- エラー時のクリーンアップ

## 🚀 次のステップ

1. **`src_explained/`** で詳細なコード解説を確認
2. **特定の機能**（パイプ、リダイレクトなど）を深掘り
3. **デバッグ方法**を学習してトラブルシューティング
4. **テストケース**を作成して動作確認

---

_このドキュメントは、minishell プロジェクトの全体像を理解するためのガイドです。_
_詳細な実装については、各モジュールの解説ファイルを参照してください。_
