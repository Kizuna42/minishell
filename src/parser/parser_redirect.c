/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/01 04:07:57 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_node_type	get_redirect_node_type(t_token_type token_type)
{
	if (token_type == TOKEN_REDIRECT_IN)
		return (NODE_REDIRECT_IN);
	else if (token_type == TOKEN_REDIRECT_OUT)
		return (NODE_REDIRECT_OUT);
	else if (token_type == TOKEN_REDIRECT_APPEND)
		return (NODE_REDIRECT_APPEND);
	else
		return (NODE_REDIRECT_HEREDOC);
}

static char	*handle_wildcard_expansion(char *filename)
{
	char	**wildcard_matches;
	char	*result;

	wildcard_matches = expand_wildcard(filename);
	if (!wildcard_matches)
		return (ft_strdup(filename));
	if (wildcard_matches[1])
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(filename, STDERR_FILENO);
		ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
		free_args(wildcard_matches);
		return (NULL);
	}
	result = ft_strdup(wildcard_matches[0]);
	free_args(wildcard_matches);
	return (result);
}

static char	*expand_redirect_filename(char *filename,
	t_token_type redirect_type)
{
	if (redirect_type == TOKEN_REDIRECT_HEREDOC)
		return (ft_strdup(filename));
	if (ft_strchr(filename, '*') && !ft_strchr(filename, '\x01')
		&& !ft_strchr(filename, '\x02'))
		return (handle_wildcard_expansion(filename));
	return (remove_quote_markers(filename));
}

static int	set_redirect_filename(t_ast_node *redirect_node, t_token **tokens,
	t_token_type redirect_type)
{
	if (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		redirect_node->filename = expand_redirect_filename((*tokens)->value,
				redirect_type);
		if (!redirect_node->filename)
			return (0);
		*tokens = (*tokens)->next;
	}
	return (1);
}

t_ast_node	*create_redirect_node(t_token **tokens)
{
	t_ast_node		*redirect_node;
	t_token_type	redirect_type;

	redirect_type = (*tokens)->type;
	*tokens = (*tokens)->next;
	redirect_node = create_ast_node(get_redirect_node_type(redirect_type));
	if (!redirect_node)
		return (NULL);
	if (!set_redirect_filename(redirect_node, tokens, redirect_type))
	{
		free(redirect_node);
		return (NULL);
	}
	return (redirect_node);
}
