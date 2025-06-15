/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_multi.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 04:51:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/15 19:28:33 by kizuna           ###   ########.fr       */
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

static void	write_heredoc_line(int pipefd, char *line, char *delimiter,
	t_minishell *shell)
{
	char	*expanded_line;

	if (ft_strchr(delimiter, '\x01') == NULL
		&& ft_strchr(delimiter, '\x02') == NULL)
	{
		expanded_line = expand_variables(line, shell);
		write(pipefd, expanded_line, ft_strlen(expanded_line));
		free(expanded_line);
	}
	else
		write(pipefd, line, ft_strlen(line));
	write(pipefd, "\n", 1);
}

static int	read_until_delimiter(int pipefd, char *delimiter,
	char *trimmed_delimiter, t_minishell *shell)
{
	char	*line;

	while (1)
	{
		line = read_heredoc_input();
		if (!line)
		{
			print_heredoc_warning(trimmed_delimiter);
			return (0);
		}
		if (ft_strncmp(line, trimmed_delimiter,
				ft_strlen(trimmed_delimiter)) == 0
			&& ft_strlen(line) == ft_strlen(trimmed_delimiter))
		{
			free(line);
			return (0);
		}
		write_heredoc_line(pipefd, line, delimiter, shell);
		free(line);
	}
}

static int	handle_single_heredoc_input(int pipefd, char *delimiter,
	char *trimmed_delimiter, t_minishell *shell)
{
	char	*line;

	line = read_heredoc_input();
	if (!line)
	{
		print_heredoc_warning(trimmed_delimiter);
		return (0);
	}
	if (ft_strncmp(line, trimmed_delimiter, ft_strlen(trimmed_delimiter)) != 0
		|| ft_strlen(line) != ft_strlen(trimmed_delimiter))
		write_heredoc_line(pipefd, line, delimiter, shell);
	free(line);
	return (0);
}

int	process_single_heredoc(char *delimiter, t_minishell *shell,
	int *final_fd)
{
	int		pipefd[2];
	char	*trimmed_delimiter;

	if (pipe(pipefd) == -1)
		return (1);
	trimmed_delimiter = remove_quote_markers(delimiter);
	handle_single_heredoc_input(pipefd[1], delimiter, trimmed_delimiter, shell);
	free(trimmed_delimiter);
	close(pipefd[1]);
	if (*final_fd >= 0)
		close(*final_fd);
	*final_fd = pipefd[0];
	return (0);
}
