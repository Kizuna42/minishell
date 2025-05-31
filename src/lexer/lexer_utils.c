/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/31 19:52:12 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*extract_continuous_word(char *input, int *i)
{
	char	*result;
	char	*temp;
	char	*part;

	result = ft_strdup("");
	while (input[*i] && !is_whitespace(input[*i]) && !is_operator(input[*i]))
	{
		if (input[*i] == '$' && is_quote(input[*i + 1]))
		{
			(*i)++;
			part = extract_quoted_string(input, i);
		}
		else if (is_quote(input[*i]))
			part = extract_quoted_string(input, i);
		else
			part = extract_word(input, i);
		if (part)
		{
			temp = ft_strjoin(result, part);
			free(result);
			free(part);
			result = temp;
		}
	}
	return (result);
}

void	process_token(char *input, int *i, t_token **tokens, int advance)
{
	t_token	*token;
	char	*value;

	if (is_operator(input[*i]))
	{
		token = create_token(get_operator_type(input + *i, &advance),
				ft_substr(input, *i, advance));
		*i += advance;
		add_token(tokens, token);
		return ;
	}
	else
		value = extract_continuous_word(input, i);
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
