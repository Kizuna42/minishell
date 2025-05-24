/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/24 18:49:58 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

#ifdef BONUS

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
	if (*str && (*pattern == *str || *pattern == '?'))
		return (match_pattern(str + 1, pattern + 1));
	return (0);
}

char	**expand_wildcards(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**result;
	int				count;
	int				i;

	dir = opendir(".");
	if (!dir)
		return (NULL);
	result = malloc(sizeof(char *) * 1000);
	if (!result)
		return (NULL);
	count = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] != '.' && match_pattern(entry->d_name, pattern))
		{
			result[count] = ft_strdup(entry->d_name);
			count++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	result[count] = NULL;
	return (result);
}

#endif
