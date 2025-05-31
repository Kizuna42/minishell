/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 04:01:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/01 04:03:54 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	process_directory_entry(struct dirent *entry, char *pattern,
	char **matches, int *i)
{
	if (should_match_entry(pattern, entry->d_name))
		matches[(*i)++] = ft_strdup(entry->d_name);
}

int	count_matches(char *pattern)
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
		if (should_match_entry(pattern, entry->d_name))
			count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}

char	**fill_matches(char *pattern, int count)
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
