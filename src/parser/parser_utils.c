/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/24 19:40:16 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**allocate_args_array(int arg_count, t_ast_node *cmd)
{
	char	**args;

	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
	{
		free_ast(cmd);
		return (NULL);
	}
	return (args);
}

t_ast_node	*parse_command(t_token **tokens)
{
	t_ast_node	*cmd;
	int			arg_count;
	int			i;

	if (!tokens || !*tokens || (*tokens)->type != TOKEN_WORD)
		return (NULL);
	cmd = create_ast_node(NODE_COMMAND);
	if (!cmd)
		return (NULL);
	arg_count = count_args(*tokens);
	cmd->args = allocate_args_array(arg_count, cmd);
	if (!cmd->args)
		return (NULL);
	i = 0;
	while (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		cmd->args[i] = ft_strdup((*tokens)->value);
		i++;
		*tokens = (*tokens)->next;
	}
	cmd->args[i] = NULL;
	return (cmd);
}

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
	return (parse_pipeline(&tokens));
}
