/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/31 21:08:46 by kizuna           ###   ########.fr       */
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

static void	sort_matches(char **matches, int count)
{
	int		i;
	int		j;
	char	*temp;
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
			max_len = (len1 > len2) ? len1 : len2;
			if (ft_strncmp(matches[i], matches[j], max_len) > 0)
			{
				temp = matches[i];
				matches[i] = matches[j];
				matches[j] = temp;
			}
			j++;
		}
		i++;
	}
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
		if (pattern[0] == '.' && entry->d_name[0] == '.')
		{
			if (match_pattern(entry->d_name, pattern))
				count++;
		}
		else if (pattern[0] != '.' && entry->d_name[0] != '.')
		{
			if (match_pattern(entry->d_name, pattern))
				count++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}

static void	process_directory_entry(struct dirent *entry, char *pattern,
	char **matches, int *i)
{
	if (pattern[0] == '.' && entry->d_name[0] == '.')
	{
		if (match_pattern(entry->d_name, pattern))
			matches[(*i)++] = ft_strdup(entry->d_name);
	}
	else if (pattern[0] != '.' && entry->d_name[0] != '.')
	{
		if (match_pattern(entry->d_name, pattern))
			matches[(*i)++] = ft_strdup(entry->d_name);
	}
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
	sort_matches(matches, i);
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
