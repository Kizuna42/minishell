/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 04:56:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/15 18:54:29 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	process_single_heredoc_redirect(t_ast_node *redirect,
	t_minishell *shell, int *final_fd)
{
	int	temp_fd;
	int	result;

	temp_fd = -1;
	result = process_single_heredoc(redirect->filename, shell, &temp_fd);
	if (result != 0)
	{
		if (temp_fd >= 0)
			close(temp_fd);
		if (*final_fd >= 0)
			close(*final_fd);
		return (result);
	}
	if (*final_fd >= 0)
		close(*final_fd);
	*final_fd = temp_fd;
	return (0);
}

static int	process_heredoc_redirects(t_ast_node **redirects, int count,
	t_minishell *shell, int *final_fd)
{
	int	i;
	int	result;

	i = count - 1;
	while (i >= 0)
	{
		if (redirects[i]->type == NODE_REDIRECT_HEREDOC)
		{
			result = process_single_heredoc_redirect(redirects[i],
					shell, final_fd);
			if (result != 0)
				return (result);
		}
		i--;
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
