/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/25 21:12:15 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

static int	count_non_empty_args(char **args)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (args[i])
	{
		if (args[i][0] != '\0')
			count++;
		i++;
	}
	return (count);
}

static void	copy_non_empty_args(char **args, char **result)
{
	int		i;
	int		j;
	char	*cleaned;

	i = 0;
	j = 0;
	while (args[i])
	{
		if (args[i][0] != '\0')
		{
			cleaned = remove_quote_markers(args[i]);
			result[j++] = cleaned;
		}
		i++;
	}
	result[j] = NULL;
}

char	**remove_empty_args(char **args)
{
	char	**result;
	int		count;

	count = count_non_empty_args(args);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	copy_non_empty_args(args, result);
	return (result);
}
