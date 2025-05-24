/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/24 19:55:03 by kizuna           ###   ########.fr       */
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
	char	quote_char;
	int		start;
	int		len;
	char	*result;
	char	*temp;

	quote_char = str[*i];
	start = ++(*i);
	while (str[*i] && str[*i] != quote_char)
		(*i)++;
	if (str[*i] == quote_char)
	{
		len = *i - start;
		(*i)++;
		result = ft_substr(str, start, len);
		if (quote_char == '\'')
		{
			temp = ft_strjoin("\x01", result);
			free(result);
			result = ft_strjoin(temp, "\x01");
			free(temp);
		}
		return (result);
	}
	return (NULL);
}
