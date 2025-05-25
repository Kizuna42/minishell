/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/25 21:25:14 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*add_quote_markers(char *result, char quote_char)
{
	char	*temp;
	char	*final_result;

	if (quote_char == '\'')
	{
		temp = ft_strjoin("\x01", result);
		free(result);
		final_result = ft_strjoin(temp, "\x01");
		free(temp);
	}
	else if (quote_char == '"')
	{
		temp = ft_strjoin("\x02", result);
		free(result);
		final_result = ft_strjoin(temp, "\x02");
		free(temp);
	}
	else
		final_result = result;
	return (final_result);
}

char	*extract_quoted_string_split(char *str, int *i)
{
	char	quote_char;
	int		start;
	int		len;
	char	*result;

	quote_char = str[*i];
	start = ++(*i);
	while (str[*i] && str[*i] != quote_char)
		(*i)++;
	if (str[*i] == quote_char)
	{
		len = *i - start;
		(*i)++;
		result = ft_substr(str, start, len);
		return (add_quote_markers(result, quote_char));
	}
	return (NULL);
}
