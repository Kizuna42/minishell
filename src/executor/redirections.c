/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/30 19:18:59 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_input_redirect(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		if (ft_strchr(filename, '*'))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(filename, STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		}
		else
			perror(filename);
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	handle_output_redirect(char *filename, int append)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	read_heredoc_lines(int pipefd[2], char *delimiter)
{
	char	*line;
	char	*trimmed_delimiter;

	trimmed_delimiter = remove_quote_markers(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, trimmed_delimiter,
				ft_strlen(trimmed_delimiter)) == 0
			&& ft_strlen(line) == ft_strlen(trimmed_delimiter))
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	free(trimmed_delimiter);
	return (0);
}

int	handle_heredoc(char *delimiter)
{
	int		pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	read_heredoc_lines(pipefd, delimiter);
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	return (0);
}

int	execute_redirections(t_ast_node *node, t_minishell *shell)
{
	t_ast_node	*redirects[10];
	t_ast_node	*cmd_node;
	int			count;
	int			result;

	count = 0;
	cmd_node = collect_redirections(node, redirects, &count);
	result = execute_redirect_list(redirects, count);
	if (result != 0)
		return (result);
	return (execute_ast(cmd_node, shell));
}
