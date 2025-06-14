/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/15 04:09:46 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_ast_node	*create_pipe_node(t_ast_node *left, t_token **tokens)
{
	t_ast_node	*pipe_node;

	*tokens = (*tokens)->next;
	pipe_node = create_ast_node(NODE_PIPE);
	if (!pipe_node)
	{
		free_ast(left);
		return (NULL);
	}
	pipe_node->left = left;
	pipe_node->right = parse_pipeline(tokens);
	if (!pipe_node->right)
	{
		free_ast(pipe_node);
		return (NULL);
	}
	return (pipe_node);
}

static t_ast_node	*parse_subshell(t_token **tokens)
{
	t_ast_node	*subshell_node;
	t_ast_node	*inner_node;

	if (!*tokens || (*tokens)->type != TOKEN_LPAREN)
		return (NULL);
	*tokens = (*tokens)->next;
	inner_node = parse_logical_ops(tokens);
	if (!inner_node)
		return (NULL);
	if (!*tokens || (*tokens)->type != TOKEN_RPAREN)
	{
		free_ast(inner_node);
		return (NULL);
	}
	*tokens = (*tokens)->next;
	subshell_node = create_ast_node(NODE_SUBSHELL);
	if (!subshell_node)
	{
		free_ast(inner_node);
		return (NULL);
	}
	subshell_node->left = inner_node;
	return (subshell_node);
}

t_ast_node	*parse_pipeline(t_token **tokens)
{
	t_ast_node	*left;

	if (*tokens && (*tokens)->type == TOKEN_LPAREN)
		left = parse_subshell(tokens);
	else
		left = parse_command(tokens);
	if (!left)
		return (NULL);
	left = parse_redirections(tokens, left);
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
		return (create_pipe_node(left, tokens));
	return (left);
}

t_ast_node	*parse(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	return (parse_logical_ops(&tokens));
}
