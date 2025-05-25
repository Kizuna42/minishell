/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/25 21:25:23 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&'
		|| c == '(' || c == ')');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

static t_token_type	get_double_operator_type(char *str, int *advance)
{
	if (str[0] == '|' && str[1] == '|')
	{
		*advance = 2;
		return (TOKEN_OR);
	}
	else if (str[0] == '&' && str[1] == '&')
	{
		*advance = 2;
		return (TOKEN_AND);
	}
	else if (str[0] == '<' && str[1] == '<')
	{
		*advance = 2;
		return (TOKEN_REDIRECT_HEREDOC);
	}
	else if (str[0] == '>' && str[1] == '>')
	{
		*advance = 2;
		return (TOKEN_REDIRECT_APPEND);
	}
	return (TOKEN_WORD);
}

t_token_type	get_operator_type(char *str, int *advance)
{
	t_token_type	type;

	type = get_double_operator_type(str, advance);
	if (type != TOKEN_WORD)
		return (type);
	*advance = 1;
	if (str[0] == '|')
		return (TOKEN_PIPE);
	else if (str[0] == '<')
		return (TOKEN_REDIRECT_IN);
	else if (str[0] == '>')
		return (TOKEN_REDIRECT_OUT);
	else if (str[0] == '(')
		return (TOKEN_LPAREN);
	else if (str[0] == ')')
		return (TOKEN_RPAREN);
	return (TOKEN_WORD);
}

char	*extract_quoted_string(char *str, int *i)
{
	return (extract_quoted_string_split(str, i));
}
