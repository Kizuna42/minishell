/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/01 18:46:56 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 * 🔧 シェル初期化関数
 *
 * minishellの実行に必要な全ての初期設定を行う
 *
 * 処理内容:
 * 1. 環境変数リストの構築（envp配列 → 連結リスト）
 * 2. execve用の環境変数配列の作成
 * 3. 終了ステータスの初期化
 * 4. 標準入出力のバックアップ（リダイレクト復元用）
 *
 * @param shell: 初期化するシェル構造体
 * @param envp: システムから受け取った環境変数配列
 */
void	init_minishell(t_minishell *shell, char **envp)
{
	// 🌍 環境変数リストの構築
	// envp配列を連結リストに変換（追加・削除・検索を効率化）
	shell->env_list = init_env(envp);

	// 📋 execve用環境変数配列の作成
	// 外部コマンド実行時に必要な配列形式に変換
	shell->envp = env_to_array(shell);

	// 📊 終了ステータスの初期化
	// $? の初期値は0（成功）
	shell->last_exit_status = 0;

	// 💾 標準入出力のバックアップ
	// リダイレクト後に元の状態に復元するため
	shell->stdin_backup = dup(STDIN_FILENO);   // 標準入力のコピー
	shell->stdout_backup = dup(STDOUT_FILENO); // 標準出力のコピー
}

/*
 * 🔄 メインループ - シェルの心臓部
 *
 * ユーザーからの入力を無限ループで処理する
 *
 * 処理フロー:
 * 1. シグナル状態のリセット
 * 2. ユーザー入力の取得（readline or get_next_line）
 * 3. 入力の処理（字句解析 → 構文解析 → 実行）
 * 4. 終了条件のチェック
 *
 * @param shell: シェル構造体
 * @return: 最終的な終了ステータス
 */
int	handle_input_loop(t_minishell *shell)
{
	char	*input;
	int		should_exit;

	while (1)
	{
		// 🔄 シグナル状態のリセット
		// 各コマンド実行前にクリアな状態にする
		g_signal_status = 0;

		// 📥 ユーザー入力の取得
		if (isatty(STDIN_FILENO))
		{
			// 🖥️ 対話モード: readlineでプロンプト表示
			// - 履歴機能付き
			// - 行編集機能付き（矢印キー、Ctrl+A/E等）
			input = readline(PROMPT);
		}
		else
		{
			// 📄 非対話モード: ファイルやパイプからの入力
			// - スクリプト実行時
			// - echo "command" | ./minishell 等
			input = get_next_line(STDIN_FILENO);
			if (input && input[ft_strlen(input) - 1] == '\n')
				input[ft_strlen(input) - 1] = '\0'; // 改行文字を削除
		}

		// 🔍 入力の処理と終了判定
		should_exit = handle_readline_input(shell, input);
		if (should_exit)
			break ;
	}
	return (shell->last_exit_status);
}

/*
 * 🎯 AST実行の統合処理
 *
 * パースされたASTを実行し、適切なクリーンアップを行う
 *
 * @param ast: 実行するAST
 * @param shell: シェル構造体
 */
static void	execute_parsed_ast(t_ast_node *ast, t_minishell *shell)
{
	if (ast)
	{
		// ⚡ AST実行
		// - コマンド、パイプ、リダイレクト等を再帰的に処理
		// - 終了ステータスを更新
		shell->last_exit_status = execute_ast(ast, shell);

		// 🧹 ASTメモリの解放
		// - 再帰的に全ノードを解放
		// - メモリリーク防止
		free_ast(ast);
	}
	else
	{
		// ❌ パースエラー時
		shell->last_exit_status = 1;
	}
}

/*
 * ✅ 構文検証とパース処理
 *
 * トークン列の構文チェックとAST構築を行う
 *
 * @param tokens: 検証・パースするトークン列
 * @param ast: 構築されたASTを格納するポインタ
 * @return: 0=成功, 2=構文エラー
 */
static int	validate_and_parse(t_token *tokens, t_ast_node **ast)
{
	// 🔍 構文検証
	// - 演算子の連続チェック
	// - 括弧の対応チェック
	// - リダイレクトの妥当性チェック
	if (!validate_syntax(tokens))
		return (2); // 構文エラー

	// 🌳 AST構築
	// - トークン列を再帰下降パーサーで解析
	// - 演算子の優先度を考慮した木構造を構築
	*ast = parse(tokens);
	return (0);
}

/*
 * 🎮 入力処理のメイン関数
 *
 * ユーザー入力を受け取り、完全な処理パイプラインを実行する
 *
 * 処理パイプライン:
 * 入力文字列 → 字句解析 → 構文検証 → 構文解析 → 実行
 *
 * @param input: ユーザーが入力した文字列
 * @param shell: シェル構造体
 */
void	process_input(char *input, t_minishell *shell)
{
	t_token		*tokens;
	t_ast_node	*ast;
	int			syntax_error;

	// 🚫 空入力のチェック
	if (!input || !*input)
		return ;

	// 📚 履歴への追加（対話モードのみ）
	if (isatty(STDIN_FILENO))
		add_history(input);

	// 🔤 字句解析（Lexical Analysis）
	// 入力文字列をトークンに分割
	// 例: "ls -la | grep .c" → [WORD:"ls"] [WORD:"-la"] [PIPE] [WORD:"grep"] [WORD:".c"]
	tokens = tokenize(input);
	if (!tokens)
		return ; // トークン化失敗

	// ✅ 構文検証とパース
	syntax_error = validate_and_parse(tokens, &ast);
	if (syntax_error)
	{
		// ❌ 構文エラー時の処理
		shell->last_exit_status = syntax_error;
		free_tokens(tokens);
		return ;
	}

	// ⚡ AST実行
	execute_parsed_ast(ast, shell);

	// 🧹 トークンメモリの解放
	free_tokens(tokens);
}

/*
 * 💡 設計のポイント:
 *
 * 1. **明確な責任分離**
 *    - 各関数が単一の責任を持つ
 *    - 処理の流れが追いやすい
 *
 * 2. **エラーハンドリング**
 *    - 各段階でのエラーチェック
 *    - 適切なメモリ解放
 *
 * 3. **メモリ管理**
 *    - 動的メモリの確実な解放
 *    - リークフリーな実装
 *
 * 4. **モード対応**
 *    - 対話モード vs 非対話モード
 *    - それぞれに適した入力処理
 */
