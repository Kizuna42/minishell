/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 18:01:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/15 19:50:45 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*read_from_tty(void)
{
	char	*line;
	int		tty_fd;
	int		saved_stdin;

	tty_fd = open("/dev/tty", O_RDONLY);
	if (tty_fd < 0)
		return (NULL);
	saved_stdin = dup(STDIN_FILENO);
	dup2(tty_fd, STDIN_FILENO);
	line = readline("> ");
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	close(tty_fd);
	return (line);
}

char	*read_heredoc_input(void)
{
	char	*line;

	if (isatty(STDIN_FILENO))
	{
		line = readline("> ");
		return (line);
	}
	else
	{
		line = read_from_tty();
		if (line)
			return (line);
		line = get_next_line(STDIN_FILENO);
		if (line && line[ft_strlen(line) - 1] == '\n')
			line[ft_strlen(line) - 1] = '\0';
		return (line);
	}
}
