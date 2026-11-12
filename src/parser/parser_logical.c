/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_logical.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/01 03:49:35 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_ast_node	*new_list_node(t_ast_node *left, t_token **tokens)
{
	t_ast_node	*node;
	t_node_type	type;

	if ((*tokens)->type == TOKEN_AND)
		type = NODE_AND;
	else if ((*tokens)->type == TOKEN_OR)
		type = NODE_OR;
	else if ((*tokens)->type == TOKEN_SEMICOLON)
		type = NODE_SEMICOLON;
	else
		type = NODE_BACKGROUND;
	node = create_ast_node(type);
	if (!node)
	{
		free_ast(left);
		return (NULL);
	}
	node->left = left;
	*tokens = (*tokens)->next;
	return (node);
}

/*
** and_or : pipeline { ('&&' | '||') pipeline }
** Left-associative with equal precedence, as in bash.
*/
static t_ast_node	*parse_and_or(t_token **tokens)
{
	t_ast_node	*left;
	t_ast_node	*node;

	left = parse_pipeline(tokens);
	while (left && *tokens && ((*tokens)->type == TOKEN_AND
			|| (*tokens)->type == TOKEN_OR))
	{
		node = new_list_node(left, tokens);
		if (!node)
			return (NULL);
		node->right = parse_pipeline(tokens);
		if (!node->right)
		{
			free_ast(node);
			return (NULL);
		}
		left = node;
	}
	return (left);
}

static int	is_list_separator(t_token *token)
{
	return (token && (token->type == TOKEN_SEMICOLON
			|| token->type == TOKEN_BACKGROUND));
}

/*
** list : and_or { (';' | '&') and_or } [';' | '&']
** ';' and '&' bind looser than '&&' / '||'. A trailing separator
** (at end of input or before ')') is accepted, as in bash.
*/
t_ast_node	*parse_logical_ops(t_token **tokens)
{
	t_ast_node	*left;
	t_ast_node	*node;

	left = parse_and_or(tokens);
	while (left && is_list_separator(*tokens))
	{
		if (!(*tokens)->next || (*tokens)->next->type == TOKEN_RPAREN)
		{
			*tokens = (*tokens)->next;
			return (left);
		}
		node = new_list_node(left, tokens);
		if (!node)
			return (NULL);
		node->right = parse_and_or(tokens);
		if (!node->right)
		{
			free_ast(node);
			return (NULL);
		}
		left = node;
	}
	return (left);
}
