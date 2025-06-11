/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/11 20:38:37 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*read_interactive_input(void)
{
	char	*input;
	int		was_interrupted;

	while (1)
	{
		g_signal_status = 0;
		input = readline(PROMPT);
		was_interrupted = (g_signal_status == SIGINT);
		g_signal_status = 0;
		if (was_interrupted && input)
		{
			free(input);
			continue ;
		}
		if (was_interrupted && !input)
			continue ;
		return (input);
	}
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
