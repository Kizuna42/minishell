/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_multi_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 18:42:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/15 18:59:44 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	print_heredoc_eof_warning(char *trimmed_delimiter)
{
	ft_putstr_fd("minishell: warning: here-document at line 1 delimited by ",
		STDERR_FILENO);
	ft_putstr_fd("end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd(trimmed_delimiter, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}

int	read_and_discard_heredoc(char *delimiter, t_minishell *shell)
{
	char	*line;
	char	*trimmed_delimiter;

	trimmed_delimiter = remove_quote_markers(delimiter);
	while (1)
	{
		line = read_heredoc_input();
		if (!line)
		{
			print_heredoc_eof_warning(trimmed_delimiter);
			free(trimmed_delimiter);
			return (0);
		}
		if (ft_strncmp(line, trimmed_delimiter,
				ft_strlen(trimmed_delimiter)) == 0
			&& ft_strlen(line) == ft_strlen(trimmed_delimiter))
		{
			free(line);
			break ;
		}
		free(line);
	}
	free(trimmed_delimiter);
	return (0);
}

int	find_last_heredoc_index(t_ast_node **redirects, int count)
{
	int	i;
	int	last_heredoc_index;

	last_heredoc_index = -1;
	i = count - 1;
	while (i >= 0)
	{
		if (redirects[i]->type == NODE_REDIRECT_HEREDOC)
		{
			if (last_heredoc_index == -1)
				last_heredoc_index = i;
		}
		i--;
	}
	return (last_heredoc_index);
}
