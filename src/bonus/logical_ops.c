/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_ops.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/24 18:49:31 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

#ifdef BONUS

t_ast_node	*parse_logical_ops(t_token **tokens)
{
	t_ast_node	*left;
	t_ast_node	*logical_node;

	left = parse_pipeline(tokens);
	if (!left)
		return (NULL);
	if (*tokens && ((*tokens)->type == TOKEN_AND
			|| (*tokens)->type == TOKEN_OR))
	{
		if ((*tokens)->type == TOKEN_AND)
			logical_node = create_ast_node(NODE_AND);
		else
			logical_node = create_ast_node(NODE_OR);
		if (!logical_node)
		{
			free_ast(left);
			return (NULL);
		}
		logical_node->left = left;
		*tokens = (*tokens)->next;
		logical_node->right = parse_logical_ops(tokens);
		if (!logical_node->right)
		{
			free_ast(logical_node);
			return (NULL);
		}
		return (logical_node);
	}
	return (left);
}

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

#endif
