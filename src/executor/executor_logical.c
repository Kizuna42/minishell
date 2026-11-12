/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_logical.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/01 04:03:54 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Runs the left side, publishes its status as $? for the right side,
** then decides whether the right side runs ('&&' / '||' short-circuit).
*/
int	execute_logical_ops(t_ast_node *node, t_minishell *shell)
{
	int	status;

	status = execute_ast(node->left, shell);
	shell->last_exit_status = status;
	if (node->type == NODE_AND && status != 0)
		return (status);
	if (node->type == NODE_OR && status == 0)
		return (status);
	return (execute_ast(node->right, shell));
}
