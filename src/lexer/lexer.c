/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/24 18:55:15 by kizuna           ###   ########.fr       */
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

	quote_char = str[*i];
	start = ++(*i);
	while (str[*i] && str[*i] != quote_char)
		(*i)++;
	if (str[*i] == quote_char)
	{
		len = *i - start;
		(*i)++;
		return (ft_substr(str, start, len));
	}
	return (NULL);
}

void	process_token(char *input, int *i, t_token **tokens, int advance)
{
	t_token	*token;
	char	*value;

	if (is_quote(input[*i]))
		value = extract_quoted_string(input, i);
	else if (is_operator(input[*i]))
	{
		token = create_token(get_operator_type(input + *i, &advance),
				ft_substr(input, *i, advance));
		*i += advance;
		add_token(tokens, token);
		return ;
	}
	else
		value = extract_word(input, i);
	token = create_token(TOKEN_WORD, value);
	add_token(tokens, token);
}

char	*extract_word(char *str, int *i)
{
	int	start;

	start = *i;
	while (str[*i] && !is_whitespace(str[*i]) && !is_operator(str[*i])
		&& !is_quote(str[*i]))
		(*i)++;
	return (ft_substr(str, start, *i - start));
}
