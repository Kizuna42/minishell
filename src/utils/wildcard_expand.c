/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_expand.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/25 21:05:07 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	copy_wildcard_matches(char **wildcard_matches, char **result,
	int *result_idx)
{
	int	j;

	j = 0;
	while (wildcard_matches[j])
	{
		result[*result_idx] = ft_strdup(wildcard_matches[j]);
		(*result_idx)++;
		j++;
	}
}

char	**fill_wildcard_result(char **args, char **result)
{
	char	**wildcard_matches;
	int		i;
	int		result_idx;

	i = 0;
	result_idx = 0;
	while (args[i])
	{
		wildcard_matches = expand_wildcard(args[i]);
		if (wildcard_matches)
		{
			copy_wildcard_matches(wildcard_matches, result, &result_idx);
			free_args(wildcard_matches);
		}
		else
		{
			result[result_idx] = ft_strdup(args[i]);
			result_idx++;
		}
		i++;
	}
	result[result_idx] = NULL;
	return (result);
}

static int	count_wildcard_matches(char **args)
{
	char	**wildcard_matches;
	int		total_count;
	int		i;
	int		j;

	total_count = 0;
	i = 0;
	while (args[i])
	{
		wildcard_matches = expand_wildcard(args[i]);
		if (wildcard_matches)
		{
			j = 0;
			while (wildcard_matches[j])
				j++;
			total_count += j;
			free_args(wildcard_matches);
		}
		else
			total_count++;
		i++;
	}
	return (total_count);
}

char	**expand_with_wildcards(char **args)
{
	char	**result;
	int		total_count;

	total_count = count_wildcard_matches(args);
	result = malloc(sizeof(char *) * (total_count + 1));
	if (!result)
		return (NULL);
	return (fill_wildcard_result(args, result));
}
