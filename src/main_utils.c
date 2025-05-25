/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/25 21:45:01 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_minishell(t_minishell *shell, char **envp)
{
	shell->env_list = init_env(envp);
	shell->envp = env_to_array(shell);
	shell->last_exit_status = 0;
	shell->stdin_backup = dup(STDIN_FILENO);
	shell->stdout_backup = dup(STDOUT_FILENO);
}

int	handle_input_loop(t_minishell *shell)
{
	char	*input;
	int		should_exit;

	while (1)
	{
		g_signal_status = 0;
		if (isatty(STDIN_FILENO))
			input = readline(PROMPT);
		else
		{
			input = get_next_line(STDIN_FILENO);
			if (input && input[ft_strlen(input) - 1] == '\n')
				input[ft_strlen(input) - 1] = '\0';
		}
		should_exit = handle_readline_input(shell, input);
		if (should_exit)
			break ;
	}
	return (shell->last_exit_status);
}

static void	execute_parsed_ast(t_ast_node *ast, t_minishell *shell)
{
	if (ast)
	{
		shell->last_exit_status = execute_ast(ast, shell);
		free_ast(ast);
	}
	else
	{
		shell->last_exit_status = 1;
	}
}

static int	validate_and_parse(t_token *tokens, t_ast_node **ast)
{
	if (!validate_syntax(tokens))
		return (2);
	*ast = parse(tokens);
	return (0);
}

void	process_input(char *input, t_minishell *shell)
{
	t_token		*tokens;
	t_ast_node	*ast;
	int			syntax_error;

	if (!input || !*input)
		return ;
	if (isatty(STDIN_FILENO))
		add_history(input);
	tokens = tokenize(input);
	if (!tokens)
		return ;
	syntax_error = validate_and_parse(tokens, &ast);
	if (syntax_error)
	{
		shell->last_exit_status = syntax_error;
		free_tokens(tokens);
		return ;
	}
	execute_parsed_ast(ast, shell);
	free_tokens(tokens);
}
