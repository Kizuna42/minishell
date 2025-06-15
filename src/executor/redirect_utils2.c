/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 16:32:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/15 16:34:45 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_redirect_node(t_ast_node *node)
{
	return (node && (node->type == NODE_REDIRECT_IN
			|| node->type == NODE_REDIRECT_OUT
			|| node->type == NODE_REDIRECT_APPEND
			|| node->type == NODE_REDIRECT_HEREDOC));
}

void	restore_fds(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}
