# 🔄 Minishell 実行フロー詳細解説

## 🎯 具体例で理解する処理の流れ

### 例 1: 単純なコマンド実行 `echo "Hello World"`

#### 📥 1. 入力受付 (main_utils.c)

```c
// readline()でユーザー入力を取得
input = readline("minishell$ ");
// input = "echo \"Hello World\""
```

#### 🔤 2. 字句解析 (lexer/)

```c
// tokenize()の処理
tokens = tokenize("echo \"Hello World\"");

// 結果のトークン列:
// [TOKEN_WORD:"echo"] → [TOKEN_WORD:"Hello World"] → NULL
```

**字句解析の詳細:**

```c
// lexer.c内の処理
i = 0;
while (input[i]) {
    if (input[i] == '"') {
        // ダブルクォート処理
        value = extract_quoted_string(input, &i);
        // value = "Hello World"
        token = create_token(TOKEN_WORD, value);
    }
    // ...
}
```

#### 🌳 3. 構文解析 (parser/)

```c
// parse()の処理
ast = parse(tokens);

// 結果のAST:
// NODE_COMMAND
// ├── args[0] = "echo"
// ├── args[1] = "Hello World"
// └── args[2] = NULL
```

#### ⚡ 4. 実行 (executor/)

```c
// execute_ast()の処理
if (is_builtin("echo")) {
    // ビルトインコマンドとして実行
    return builtin_echo(["echo", "Hello World"]);
}

// builtin_echo()内:
// "Hello World"を標準出力に出力
ft_putstr_fd("Hello World", STDOUT_FILENO);
ft_putstr_fd("\n", STDOUT_FILENO);
```

---

### 例 2: パイプライン `ls -la | grep .c`

#### 🔤 1. 字句解析結果

```c
// トークン列:
[TOKEN_WORD:"ls"] → [TOKEN_WORD:"-la"] → [TOKEN_PIPE] → [TOKEN_WORD:"grep"] → [TOKEN_WORD:".c"]
```

#### 🌳 2. 構文解析結果

```c
// AST構造:
NODE_PIPE
├── left: NODE_COMMAND
│   ├── args[0] = "ls"
│   ├── args[1] = "-la"
│   └── args[2] = NULL
└── right: NODE_COMMAND
    ├── args[0] = "grep"
    ├── args[1] = ".c"
    └── args[2] = NULL
```

#### ⚡ 3. パイプライン実行 (executor/pipes.c)

```c
int execute_pipeline(t_ast_node *node, t_minishell *shell) {
    int pipefd[2];
    pid_t pid1, pid2;

    // 1. パイプ作成
    pipe(pipefd);
    // pipefd[0]: 読み込み用, pipefd[1]: 書き込み用

    // 2. 左側コマンド (ls -la) の実行
    pid1 = fork();
    if (pid1 == 0) {
        // 子プロセス1: ls -la
        dup2(pipefd[1], STDOUT_FILENO);  // 標準出力をパイプに
        close(pipefd[0]);
        close(pipefd[1]);

        // ls -laを実行
        execve("/bin/ls", ["ls", "-la"], envp);
    }

    // 3. 右側コマンド (grep .c) の実行
    pid2 = fork();
    if (pid2 == 0) {
        // 子プロセス2: grep .c
        dup2(pipefd[0], STDIN_FILENO);   // 標準入力をパイプから
        close(pipefd[0]);
        close(pipefd[1]);

        // grep .cを実行
        execve("/usr/bin/grep", ["grep", ".c"], envp);
    }

    // 4. 親プロセス: パイプを閉じて子プロセスを待機
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, &status, 0);

    return WEXITSTATUS(status);
}
```

**実行の流れ:**

```
親プロセス (minishell)
├── fork() → 子プロセス1 (ls -la)
│   └── stdout → pipe[1] → pipe[0] → stdin
└── fork() → 子プロセス2 (grep .c)
    └── 結果を親プロセスに返す
```

---

### 例 3: リダイレクト `echo "test" > output.txt`

#### 🌳 1. AST 構造

```c
NODE_REDIRECT_OUT
├── left: NODE_COMMAND
│   ├── args[0] = "echo"
│   ├── args[1] = "test"
│   └── args[2] = NULL
└── filename = "output.txt"
```

#### ⚡ 2. リダイレクト実行 (executor/redirections.c)

```c
int execute_redirections(t_ast_node *node, t_minishell *shell) {
    int fd;
    int saved_stdout;

    // 1. 現在の標準出力を保存
    saved_stdout = dup(STDOUT_FILENO);

    // 2. 出力ファイルを開く
    fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // 3. 標準出力をファイルにリダイレクト
    dup2(fd, STDOUT_FILENO);
    close(fd);

    // 4. 左側のコマンドを実行
    int exit_status = execute_ast(node->left, shell);
    // echo "test"が実行され、output.txtに書き込まれる

    // 5. 標準出力を復元
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    return exit_status;
}
```

---

### 例 4: 環境変数展開 `echo "Hello $USER"`

#### 🔤 1. 字句解析での変数展開 (utils/expand.c)

```c
// tokenize()内で変数展開が実行される
char *expand_variables(char *str, t_minishell *shell) {
    char *result = malloc(BUFFER_SIZE);
    int i = 0, j = 0;

    while (str[i]) {
        if (str[i] == '$') {
            // 変数名を抽出
            char *var_name = extract_var_name(str, &i);
            // var_name = "USER"

            // 環境変数から値を取得
            char *var_value = get_env_value(var_name, shell);
            // var_value = "kizuna"

            // 結果文字列に追加
            strcpy(result + j, var_value);
            j += strlen(var_value);
        } else {
            result[j++] = str[i++];
        }
    }

    return result; // "Hello kizuna"
}
```

#### 🌳 2. 最終的な AST

```c
NODE_COMMAND
├── args[0] = "echo"
├── args[1] = "Hello kizuna"  // 変数展開済み
└── args[2] = NULL
```

---

### 例 5: ヒアドキュメント `cat << EOF`

#### 🔤 1. 字句解析結果

```c
[TOKEN_WORD:"cat"] → [TOKEN_REDIRECT_HEREDOC] → [TOKEN_WORD:"EOF"]
```

#### ⚡ 2. ヒアドキュメント処理 (executor/heredoc_utils.c)

```c
int handle_heredoc(char *delimiter, t_minishell *shell) {
    int pipefd[2];
    char *line;

    // 1. パイプ作成（一時的なデータ保存用）
    pipe(pipefd);

    // 2. ユーザーからの入力を読み取り
    while (1) {
        line = readline("> ");

        // 区切り文字をチェック
        if (strcmp(line, delimiter) == 0) {
            free(line);
            break;
        }

        // パイプに書き込み
        write(pipefd[1], line, strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }

    // 3. 書き込み側を閉じる
    close(pipefd[1]);

    // 4. 標準入力をパイプの読み込み側に変更
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);

    return 0;
}
```

**実行例:**

```bash
minishell$ cat << EOF
> This is line 1
> This is line 2
> EOF
This is line 1
This is line 2
```

---

### 例 6: 論理演算子 `ls && echo "success" || echo "failed"`

#### 🌳 1. AST 構造 (ボーナス機能)

```c
NODE_OR
├── left: NODE_AND
│   ├── left: NODE_COMMAND ("ls")
│   └── right: NODE_COMMAND ("echo", "success")
└── right: NODE_COMMAND ("echo", "failed")
```

#### ⚡ 2. 論理演算子実行 (bonus/logical_ops.c)

```c
int execute_logical_ops(t_ast_node *node, t_minishell *shell) {
    int left_status, right_status;

    switch (node->type) {
        case NODE_AND:
            // &&: 左が成功した場合のみ右を実行
            left_status = execute_ast(node->left, shell);
            if (left_status == 0) {
                return execute_ast(node->right, shell);
            }
            return left_status;

        case NODE_OR:
            // ||: 左が失敗した場合のみ右を実行
            left_status = execute_ast(node->left, shell);
            if (left_status != 0) {
                return execute_ast(node->right, shell);
            }
            return left_status;
    }
}
```

---

## 🎯 重要なポイント

### 1. **メモリ管理**

```c
// 各段階でのメモリ解放
free_tokens(tokens);    // 字句解析後
free_ast(ast);          // 実行後
cleanup_minishell();    // 終了時
```

### 2. **エラーハンドリング**

```c
// システムコールの失敗チェック
if (pipe(pipefd) == -1) {
    perror("pipe");
    return 1;
}
```

### 3. **ファイル記述子管理**

```c
// 必ず対になるopen/close, dup/restore
int saved_fd = dup(STDOUT_FILENO);
// ... リダイレクト処理 ...
dup2(saved_fd, STDOUT_FILENO);
close(saved_fd);
```

### 4. **プロセス同期**

```c
// 子プロセスの完了を適切に待機
waitpid(pid, &status, 0);
return WEXITSTATUS(status);
```

---

## 🚀 学習のコツ

1. **段階的理解**: 単純なコマンドから複雑なパイプラインへ
2. **デバッグ出力**: 各段階でのデータ構造を確認
3. **システムコール理解**: fork, exec, pipe, dup2 の動作原理
4. **メモリ追跡**: valgrind でリークチェック

このように、minishell は入力から実行まで明確な段階を経て処理されています。各段階の責任が明確に分離されているため、デバッグや機能追加が容易になっています！
