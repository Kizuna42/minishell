/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/24 18:42:33 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_signal_status = 0;

static void	init_minishell(t_minishell *shell, char **envp)
{
	shell->env_list = init_env(envp);
	shell->envp = env_to_array(shell);
	shell->last_exit_status = 0;
	shell->stdin_backup = dup(STDIN_FILENO);
	shell->stdout_backup = dup(STDOUT_FILENO);
}

void	process_input(char *input, t_minishell *shell)
{
	t_token		*tokens;
	t_ast_node	*ast;

	if (!input || !*input)
		return ;
	add_history(input);
	tokens = tokenize(input);
	if (!tokens)
		return ;
	ast = parse(tokens);
	if (ast)
	{
		shell->last_exit_status = execute_ast(ast, shell);
		free_ast(ast);
	}
	free_tokens(tokens);
}

static int	handle_input_loop(t_minishell *shell)
{
	char	*input;
	int		should_exit;

	while (1)
	{
		g_signal_status = 0;
		input = readline(PROMPT);
		should_exit = handle_readline_input(shell, input);
		if (should_exit)
			break ;
	}
	return (shell->last_exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	shell;

	(void)argc;
	(void)argv;
	init_minishell(&shell, envp);
	setup_signal_handlers();
	shell.last_exit_status = handle_input_loop(&shell);
	cleanup_minishell(&shell);
	return (shell.last_exit_status);
}
