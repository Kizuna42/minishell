/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/13 22:24:16 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_signal_in_readline(char **input)
{
	if (g_signal_status == SIGINT || g_signal_status == SIGQUIT)
	{
		g_signal_status = 0;
		if (*input)
		{
			free(*input);
			*input = NULL;
		}
		return (1);
	}
	return (0);
}

static char	*read_interactive_input(void)
{
	char	*input;

	while (1)
	{
		if (g_signal_status == SIGINT)
		{
			g_signal_status = 0;
			continue ;
		}
		input = readline(PROMPT);
		if (!input)
			return (NULL);
		if (handle_signal_in_readline(&input))
			continue ;
		if (*input == '\0')
		{
			free(input);
			return (ft_strdup(""));
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
