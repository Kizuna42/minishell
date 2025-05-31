/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/01 03:31:54 by kizuna           ###   ########.fr       */
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

static void	process_directory_entry(struct dirent *entry, char *pattern,
	char **result, int *count)
{
	if (entry->d_name[0] != '.' && match_pattern(entry->d_name, pattern))
	{
		result[*count] = ft_strdup(entry->d_name);
		(*count)++;
	}
}

char	**expand_wildcards(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**result;
	int				count;

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
		process_directory_entry(entry, pattern, result, &count);
		entry = readdir(dir);
	}
	closedir(dir);
	result[count] = NULL;
	return (result);
}

#endif
