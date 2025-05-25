/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/25 21:24:47 by kizuna           ###   ########.fr       */
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
		if (result[i] == '\x01' || result[i] == '\x02')
		{
			handle_quote_marker(result, &i, &in_single_quote);
			continue ;
		}
		if (result[i] == '$' && result[i + 1] && !in_single_quote)
			process_variable(&result, &i, shell);
		else
			i++;
	}
	return (result);
}
