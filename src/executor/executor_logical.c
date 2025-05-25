/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_logical.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/25 19:01:25 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_logical_ops(t_ast_node *node, t_minishell *shell)
{
	int	left_result;

	if (node->type == NODE_AND)
	{
		left_result = execute_ast(node->left, shell);
		if (left_result == 0)
			return (execute_ast(node->right, shell));
		return (left_result);
	}
	else if (node->type == NODE_OR)
	{
		left_result = execute_ast(node->left, shell);
		if (left_result != 0)
			return (execute_ast(node->right, shell));
		return (left_result);
	}
	return (1);
}
