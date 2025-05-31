/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logical_ops.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/01 03:57:44 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

#ifdef BONUS

static t_ast_node	*create_logical_node(t_ast_node *left, t_token **tokens)
{
	t_ast_node	*logical_node;

	if ((*tokens)->type == TOKEN_AND)
		logical_node = create_ast_node(NODE_AND);
	else if ((*tokens)->type == TOKEN_OR)
		logical_node = create_ast_node(NODE_OR);
	else if ((*tokens)->type == TOKEN_SEMICOLON)
		logical_node = create_ast_node(NODE_SEMICOLON);
	else
		logical_node = create_ast_node(NODE_BACKGROUND);
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

t_ast_node	*parse_logical_ops(t_token **tokens)
{
	t_ast_node	*left;

	left = parse_pipeline(tokens);
	if (!left)
		return (NULL);
	if (*tokens && ((*tokens)->type == TOKEN_AND
			|| (*tokens)->type == TOKEN_OR
			|| (*tokens)->type == TOKEN_SEMICOLON
			|| (*tokens)->type == TOKEN_BACKGROUND))
		return (create_logical_node(left, tokens));
	return (left);
}

static int	execute_and_or(t_ast_node *node, t_minishell *shell)
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

static int	execute_semicolon_background(t_ast_node *node, t_minishell *shell)
{
	if (node->type == NODE_SEMICOLON)
	{
		execute_ast(node->left, shell);
		return (execute_ast(node->right, shell));
	}
	else if (node->type == NODE_BACKGROUND)
	{
		execute_ast(node->left, shell);
		return (execute_ast(node->right, shell));
	}
	return (1);
}

int	execute_logical_ops(t_ast_node *node, t_minishell *shell)
{
	if (node->type == NODE_AND || node->type == NODE_OR)
		return (execute_and_or(node, shell));
	else if (node->type == NODE_SEMICOLON || node->type == NODE_BACKGROUND)
		return (execute_semicolon_background(node, shell));
	return (1);
}

#endif
