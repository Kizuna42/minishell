/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/13 22:34:27 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_pre_readline_signal(void)
{
	if (g_signal_status == SIGINT)
	{
		g_signal_status = 0;
		rl_on_new_line();
		rl_redisplay();
	}
}

static int	handle_post_readline_signal(char **input)
{
	if (g_signal_status == SIGINT)
	{
		g_signal_status = 0;
		if (*input)
			free(*input);
		rl_on_new_line();
		rl_redisplay();
		return (1);
	}
	return (0);
}

static char	*read_interactive_input(void)
{
	char	*input;

	while (1)
	{
		handle_pre_readline_signal();
		input = readline(PROMPT);
		if (handle_post_readline_signal(&input))
			continue ;
		if (!input)
			return (NULL);
		if (*input == '\0')
		{
			free(input);
			continue ;
		}
		break ;
	}
	return (input);
}

char	*read_input_line(void)
{
	char	*input;

	if (isatty(STDIN_FILENO))
		return (read_interactive_input());
	input = get_next_line(STDIN_FILENO);
	if (input && input[ft_strlen(input) - 1] == '\n')
		input[ft_strlen(input) - 1] = '\0';
	return (input);
}

void	restore_std_fds(t_minishell *shell)
{
	dup2(shell->stdin_backup, STDIN_FILENO);
	dup2(shell->stdout_backup, STDOUT_FILENO);
}

int	handle_readline_input(t_minishell *shell, char *input)
{
	if (!input)
		return (1);
	process_input(input, shell);
	restore_std_fds(shell);
	free(input);
	return (0);
}
