/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 04:56:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/15 18:59:14 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	process_heredoc_redirects(t_ast_node **redirects, int count,
	t_minishell *shell, int *final_fd)
{
	int	i;
	int	result;
	int	last_heredoc_index;

	last_heredoc_index = find_last_heredoc_index(redirects, count);
	i = 0;
	while (i < count)
	{
		if (redirects[i]->type == NODE_REDIRECT_HEREDOC)
		{
			if (i == last_heredoc_index)
				result = process_single_heredoc(redirects[i]->filename,
						shell, final_fd);
			else
				result = read_and_discard_heredoc(redirects[i]->filename,
						shell);
			if (result != 0)
				return (result);
		}
		i++;
	}
	return (0);
}

int	handle_multiple_heredocs(t_ast_node **redirects, int count,
	t_minishell *shell)
{
	int	final_fd;
	int	result;

	final_fd = -1;
	result = process_heredoc_redirects(redirects, count, shell, &final_fd);
	if (result != 0)
		return (result);
	if (final_fd >= 0)
	{
		dup2(final_fd, STDIN_FILENO);
		close(final_fd);
	}
	return (0);
}
