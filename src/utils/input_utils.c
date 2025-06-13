/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/13 21:30:30 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_signal_interrupt(char **input)
{
	if (*input)
	{
		free(*input);
		*input = NULL;
	}
}

static char	*read_interactive_input(void)
{
	char	*input;

	while (1)
	{
		g_signal_status = 0;
		input = readline(PROMPT);
		if (g_signal_status == SIGINT)
		{
			handle_signal_interrupt(&input);
			rl_on_new_line();
			rl_replace_line("", 0);
			continue ;
		}
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
