/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/01 04:11:19 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	match_pattern(char *str, char *pattern)
{
	if (!*pattern)
		return (!*str);
	if (*pattern == '*')
	{
		while (*pattern == '*')
			pattern++;
		if (!*pattern)
			return (1);
		while (*str)
		{
			if (match_pattern(str, pattern))
				return (1);
			str++;
		}
		return (match_pattern(str, pattern));
	}
	if (*str && *pattern == *str)
		return (match_pattern(str + 1, pattern + 1));
	return (0);
}

static void	swap_strings(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	sort_matches(char **matches, int count)
{
	int		i;
	int		j;
	int		len1;
	int		len2;
	int		max_len;

	i = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			len1 = ft_strlen(matches[i]);
			len2 = ft_strlen(matches[j]);
			max_len = len1;
			if (len2 > len1)
				max_len = len2;
			if (ft_strncmp(matches[i], matches[j], max_len) > 0)
				swap_strings(&matches[i], &matches[j]);
			j++;
		}
		i++;
	}
}

int	should_match_entry(char *pattern, char *entry_name)
{
	if (pattern[0] == '.' && entry_name[0] == '.')
		return (match_pattern(entry_name, pattern));
	else if (pattern[0] != '.' && entry_name[0] != '.')
		return (match_pattern(entry_name, pattern));
	return (0);
}

char	**expand_wildcard(char *pattern)
{
	int		count;
	char	**matches;

	if (!ft_strchr(pattern, '*'))
		return (NULL);
	if (ft_strchr(pattern, '\x01') || ft_strchr(pattern, '\x02'))
		return (NULL);
	count = count_matches(pattern);
	if (count == 0)
		return (NULL);
	matches = fill_matches(pattern, count);
	return (matches);
}
