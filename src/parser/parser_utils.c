/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/25 21:36:17 by kizuna           ###   ########.fr       */
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

t_ast_node	*parse_pipeline(t_token **tokens)
{
	t_ast_node	*left;

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
