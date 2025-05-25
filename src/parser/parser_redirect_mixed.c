/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect_mixed.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/25 18:28:00 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_redirect_in_command(t_token **tokens, t_ast_node **cmd)
{
	t_ast_node	*redirect_node;

	redirect_node = create_redirect_node(tokens);
	if (!redirect_node)
		return (0);
	redirect_node->left = *cmd;
	*cmd = redirect_node;
	return (1);
}

static int	process_tokens_loop(t_token **tokens, char **args,
	int *i, t_ast_node **cmd)
{
	while (*tokens && ((*tokens)->type == TOKEN_WORD
			|| is_redirect_token((*tokens)->type)))
	{
		if ((*tokens)->type == TOKEN_WORD)
		{
			args[(*i)++] = ft_strdup((*tokens)->value);
			*tokens = (*tokens)->next;
		}
		else if (is_redirect_token((*tokens)->type))
		{
			if (!handle_redirect_in_command(tokens, cmd))
				return (0);
		}
	}
	return (1);
}

t_ast_node	*parse_mixed_command(t_token **tokens)
{
	t_ast_node	*cmd;
	char		**args;
	int			arg_count;
	int			i;

	arg_count = count_word_tokens(*tokens);
	cmd = create_command_with_args(arg_count);
	if (!cmd)
		return (NULL);
	args = allocate_args_array(arg_count, cmd);
	if (!args)
		return (NULL);
	i = 0;
	if (!process_tokens_loop(tokens, args, &i, &cmd))
	{
		cleanup_on_error(args, i, cmd);
		return (NULL);
	}
	finalize_command_args(cmd, args, i);
	return (cmd);
}
