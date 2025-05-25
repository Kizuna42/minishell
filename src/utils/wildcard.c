/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/25 21:15:23 by kizuna           ###   ########.fr       */
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

static int	count_matches(char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	dir = opendir(".");
	if (!dir)
		return (0);
	count = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] == '.')
		{
			entry = readdir(dir);
			continue ;
		}
		if (match_pattern(entry->d_name, pattern))
			count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}

static void	process_directory_entry(struct dirent *entry, char *pattern,
	char **matches, int *i)
{
	if (entry->d_name[0] != '.' && match_pattern(entry->d_name, pattern))
		matches[(*i)++] = ft_strdup(entry->d_name);
}

static char	**fill_matches(char *pattern, int count)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	int				i;

	matches = malloc(sizeof(char *) * (count + 1));
	if (!matches)
		return (NULL);
	dir = opendir(".");
	if (!dir)
		return (NULL);
	i = 0;
	entry = readdir(dir);
	while (entry != NULL && i < count)
	{
		process_directory_entry(entry, pattern, matches, &i);
		entry = readdir(dir);
	}
	matches[i] = NULL;
	closedir(dir);
	return (matches);
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
