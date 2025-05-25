/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/25 21:35:57 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_redirect_token(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_REDIRECT_APPEND || type == TOKEN_REDIRECT_HEREDOC);
}

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

static char	*expand_redirect_filename(char *filename)
{
	char	**wildcard_matches;
	char	*result;

	if (ft_strchr(filename, '*') && !ft_strchr(filename, '\x01')
		&& !ft_strchr(filename, '\x02'))
	{
		wildcard_matches = expand_wildcard(filename);
		if (!wildcard_matches)
		{
			return (ft_strdup(filename));
		}
		if (wildcard_matches[1])
		{
			free_args(wildcard_matches);
			return (NULL);
		}
		result = ft_strdup(wildcard_matches[0]);
		free_args(wildcard_matches);
		return (result);
	}
	return (remove_quote_markers(filename));
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
	if (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		redirect_node->filename = expand_redirect_filename((*tokens)->value);
		if (!redirect_node->filename)
		{
			free(redirect_node);
			return (NULL);
		}
		*tokens = (*tokens)->next;
	}
	return (redirect_node);
}

int	count_word_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && (tokens->type == TOKEN_WORD
			|| is_redirect_token(tokens->type)))
	{
		if (tokens->type == TOKEN_WORD)
			count++;
		else if (is_redirect_token(tokens->type))
		{
			tokens = tokens->next;
			if (tokens && tokens->type == TOKEN_WORD)
				tokens = tokens->next;
			continue ;
		}
		tokens = tokens->next;
	}
	return (count);
}
