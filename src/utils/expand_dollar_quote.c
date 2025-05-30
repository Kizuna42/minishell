/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar_quote.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 18:30:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/30 18:48:34 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	find_closing_quote(char *str, int start)
{
	int	end;

	end = start;
	while (str[end] && str[end] != '"' && str[end] != '\x02'
		&& str[end] != '\'' && str[end] != '\x01')
		end++;
	if (str[end] != '"' && str[end] != '\x02'
		&& str[end] != '\'' && str[end] != '\x01')
		return (-1);
	return (end);
}

static void	convert_quote_markers(char *content, char *processed_content)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (content[i])
	{
		if (content[i] == '\x01')
			processed_content[j++] = '\'';
		else if (content[i] == '\x02')
			processed_content[j++] = '"';
		else
			processed_content[j++] = content[i];
		i++;
	}
	processed_content[j] = '\0';
}

static char	*process_content(char *result, int start, int end)
{
	char	*content;
	char	*processed_content;

	content = ft_substr(result, start + 2, end - start - 2);
	processed_content = malloc(ft_strlen(content) + 1);
	if (!processed_content)
	{
		free(content);
		return (NULL);
	}
	convert_quote_markers(content, processed_content);
	free(content);
	return (processed_content);
}

static char	*create_dollar_quote_result(char *result, int start, int end)
{
	char	*before;
	char	*after;
	char	*temp;
	char	*final;
	char	*processed_content;

	before = ft_substr(result, 0, start);
	processed_content = process_content(result, start, end);
	after = ft_strdup(result + end + 1);
	temp = ft_strjoin(before, processed_content);
	final = ft_strjoin(temp, after);
	free(before);
	free(processed_content);
	free(after);
	free(temp);
	return (final);
}

void	handle_dollar_quote(char **result, int *i)
{
	int		quote_end;
	char	*new_result;
	int		content_length;

	quote_end = find_closing_quote(*result, *i + 2);
	if (quote_end == -1)
	{
		(*i)++;
		return ;
	}
	content_length = quote_end - (*i + 2);
	new_result = create_dollar_quote_result(*result, *i, quote_end);
	*i += content_length;
	free(*result);
	*result = new_result;
}
