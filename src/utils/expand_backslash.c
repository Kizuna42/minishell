/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_backslash.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 23:31:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/01 02:00:06 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <string.h>

int	count_backslashes(char *str, int start)
{
	int	count;

	count = 0;
	while (str[start + count] == '\\')
		count++;
	return (count);
}

void	process_backslashes(char **result, int *i)
{
	int	backslash_count;
	int	new_count;
	int	j;

	backslash_count = count_backslashes(*result, *i);
	new_count = backslash_count / 2;
	j = 0;
	while (j < new_count)
	{
		(*result)[*i + j] = '\\';
		j++;
	}
	memmove(*result + *i + new_count, *result + *i + backslash_count,
		ft_strlen(*result + *i + backslash_count) + 1);
	*i += new_count;
}

static void	handle_backslash_dollar(char **result, int *i, int backslash_count,
	t_minishell *shell)
{
	if (backslash_count % 2 == 1)
	{
		process_backslashes(result, i);
		(*i)++;
		return ;
	}
	else
	{
		process_backslashes(result, i);
		if ((*result)[*i] == '$' && (*result)[*i + 1])
			process_variable(result, i, shell);
		else
			(*i)++;
		return ;
	}
}

void	handle_backslash(char **result, int *i, t_minishell *shell)
{
	int	backslash_count;

	backslash_count = count_backslashes(*result, *i);
	if ((*result)[*i + backslash_count] == '$')
		handle_backslash_dollar(result, i, backslash_count, shell);
	else
		process_backslashes(result, i);
}
