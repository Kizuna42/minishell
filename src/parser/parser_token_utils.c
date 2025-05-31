/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 04:07:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/01 04:07:56 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_operator_token(t_token_type type)
{
	return (type == TOKEN_PIPE || type == TOKEN_REDIRECT_IN
		|| type == TOKEN_REDIRECT_OUT || type == TOKEN_REDIRECT_APPEND
		|| type == TOKEN_REDIRECT_HEREDOC || type == TOKEN_AND
		|| type == TOKEN_OR || type == TOKEN_BACKGROUND);
}

int	is_redirect_token(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_REDIRECT_APPEND || type == TOKEN_REDIRECT_HEREDOC);
}

static void	skip_redirect_tokens(t_token **tokens)
{
	*tokens = (*tokens)->next;
	if (*tokens && (*tokens)->type == TOKEN_WORD)
		*tokens = (*tokens)->next;
}

int	count_word_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && (tokens->type == TOKEN_WORD
			|| is_redirect_token(tokens->type)))
	{
		if (tokens->type == TOKEN_WORD)
		{
			count++;
			tokens = tokens->next;
		}
		else if (is_redirect_token(tokens->type))
		{
			skip_redirect_tokens(&tokens);
		}
	}
	return (count);
}
