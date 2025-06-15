/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 19:35:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/15 18:02:45 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	print_heredoc_warning(char *delimiter)
{
	ft_putstr_fd("minishell: warning: here-document at line 1 delimited by ",
		STDERR_FILENO);
	ft_putstr_fd("end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd(delimiter, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}

static void	process_heredoc_line(int pipefd[2], char *line, char *delimiter,
	t_minishell *shell)
{
	char	*expanded_line;

	if (ft_strchr(delimiter, '\x01') == NULL
		&& ft_strchr(delimiter, '\x02') == NULL)
	{
		expanded_line = expand_variables(line, shell);
		write(pipefd[1], expanded_line, ft_strlen(expanded_line));
		free(expanded_line);
	}
	else
		write(pipefd[1], line, ft_strlen(line));
	write(pipefd[1], "\n", 1);
}

int	read_heredoc_lines(int pipefd[2], char *delimiter, t_minishell *shell)
{
	char	*line;
	char	*trimmed_delimiter;

	trimmed_delimiter = remove_quote_markers(delimiter);
	while (1)
	{
		line = read_heredoc_input();
		if (!line)
		{
			print_heredoc_warning(trimmed_delimiter);
			break ;
		}
		if (ft_strncmp(line, trimmed_delimiter,
				ft_strlen(trimmed_delimiter)) == 0
			&& ft_strlen(line) == ft_strlen(trimmed_delimiter))
		{
			free(line);
			break ;
		}
		process_heredoc_line(pipefd, line, delimiter, shell);
		free(line);
	}
	free(trimmed_delimiter);
	return (0);
}
