/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/15 04:09:46 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	execute_single_redirect(t_ast_node *redirect, t_minishell *shell)
{
	int	result;

	result = 0;
	if (redirect->type == NODE_REDIRECT_IN)
		result = handle_input_redirect(redirect->filename);
	else if (redirect->type == NODE_REDIRECT_OUT)
		result = handle_output_redirect(redirect->filename, 0);
	else if (redirect->type == NODE_REDIRECT_APPEND)
		result = handle_output_redirect(redirect->filename, 1);
	else if (redirect->type == NODE_REDIRECT_HEREDOC)
		result = handle_heredoc(redirect->filename, shell);
	return (result);
}

int	execute_redirect_list(t_ast_node **redirects, int count, t_minishell *shell)
{
	int	i;
	int	result;

	i = 0;
	while (i < count)
	{
		result = execute_single_redirect(redirects[i], shell);
		if (result != 0)
			return (result);
		i++;
	}
	return (0);
}

int	is_redirect_node(t_ast_node *node)
{
	return (node && (node->type == NODE_REDIRECT_IN
			|| node->type == NODE_REDIRECT_OUT
			|| node->type == NODE_REDIRECT_APPEND
			|| node->type == NODE_REDIRECT_HEREDOC));
}

t_ast_node	*collect_redirections(t_ast_node *node, t_ast_node **redirects,
	int *count)
{
	t_ast_node	*cmd_node;

	cmd_node = node;
	while (is_redirect_node(cmd_node))
	{
		redirects[(*count)++] = cmd_node;
		cmd_node = cmd_node->left;
	}
	return (cmd_node);
}

void	restore_fds(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}
