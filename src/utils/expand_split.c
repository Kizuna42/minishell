/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/30 18:41:40 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_quote_marker(char *result, int *i, int *in_single_quote)
{
	if (result[*i] == '\x01')
	{
		*in_single_quote = !(*in_single_quote);
		(*i)++;
	}
	else if (result[*i] == '\x02')
	{
		(*i)++;
	}
}

static void	process_character(char **result, int *i, int *in_single_quote,
	t_minishell *shell)
{
	if ((*result)[*i] == '\x01' || (*result)[*i] == '\x02')
	{
		handle_quote_marker(*result, i, in_single_quote);
		return ;
	}
	if ((*result)[*i] == '$' && ((*result)[*i + 1] == '"'
			|| (*result)[*i + 1] == '\x02' || (*result)[*i + 1] == '\''
			|| (*result)[*i + 1] == '\x01') && !(*in_single_quote))
	{
		handle_dollar_quote(result, i);
		return ;
	}
	if ((*result)[*i] == '$' && (*result)[*i + 1] && !(*in_single_quote)
		&& (*result)[*i + 1] != '"' && (*result)[*i + 1] != '\x02'
		&& (*result)[*i + 1] != '\'' && (*result)[*i + 1] != '\x01')
		process_variable(result, i, shell);
	else
		(*i)++;
}

char	*expand_variables_split(char *str, t_minishell *shell)
{
	char	*result;
	int		i;
	int		in_single_quote;

	result = ft_strdup(str);
	i = 0;
	in_single_quote = 0;
	while (result[i])
	{
		process_character(&result, &i, &in_single_quote, shell);
	}
	return (result);
}
