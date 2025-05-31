/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/01 03:49:35 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_ast_node	*create_ast_node(t_node_type type)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = NULL;
	node->filename = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

void	free_ast(t_ast_node *ast)
{
	int	i;

	if (!ast)
		return ;
	if (ast->args)
	{
		i = 0;
		while (ast->args[i])
		{
			free(ast->args[i]);
			i++;
		}
		free(ast->args);
	}
	if (ast->filename)
		free(ast->filename);
	free_ast(ast->left);
	free_ast(ast->right);
	free(ast);
}

static t_node_type	token_to_node_type(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return (NODE_PIPE);
	else if (type == TOKEN_REDIRECT_IN)
		return (NODE_REDIRECT_IN);
	else if (type == TOKEN_REDIRECT_OUT)
		return (NODE_REDIRECT_OUT);
	else if (type == TOKEN_REDIRECT_APPEND)
		return (NODE_REDIRECT_APPEND);
	else if (type == TOKEN_REDIRECT_HEREDOC)
		return (NODE_REDIRECT_HEREDOC);
	else if (type == TOKEN_AND)
		return (NODE_AND);
	else if (type == TOKEN_OR)
		return (NODE_OR);
	else if (type == TOKEN_SEMICOLON)
		return (NODE_SEMICOLON);
	else if (type == TOKEN_BACKGROUND)
		return (NODE_BACKGROUND);
	return (NODE_COMMAND);
}

int	count_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type == TOKEN_WORD)
	{
		count++;
		tokens = tokens->next;
	}
	return (count);
}

t_ast_node	*parse_redirections(t_token **tokens, t_ast_node *cmd)
{
	t_ast_node	*redirect_node;

	while (*tokens && ((*tokens)->type == TOKEN_REDIRECT_IN
			|| (*tokens)->type == TOKEN_REDIRECT_OUT
			|| (*tokens)->type == TOKEN_REDIRECT_APPEND
			|| (*tokens)->type == TOKEN_REDIRECT_HEREDOC))
	{
		redirect_node = create_ast_node(token_to_node_type((*tokens)->type));
		if (!redirect_node)
		{
			free_ast(cmd);
			return (NULL);
		}
		*tokens = (*tokens)->next;
		if (*tokens && (*tokens)->type == TOKEN_WORD)
		{
			redirect_node->filename = ft_strdup((*tokens)->value);
			*tokens = (*tokens)->next;
		}
		redirect_node->left = cmd;
		cmd = redirect_node;
	}
	return (cmd);
}
