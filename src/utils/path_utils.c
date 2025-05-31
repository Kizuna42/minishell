/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/31 21:09:03 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*search_in_paths(char **paths, char *command)
{
	char	*full_path;
	char	*temp;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, command);
		free(temp);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

int	check_file_access(char *command)
{
	struct stat	st;

	if (access(command, F_OK) != 0)
		return (127);
	if (stat(command, &st) == 0 && S_ISDIR(st.st_mode))
		return (handle_directory_error(command));
	if (access(command, X_OK) != 0)
		return (handle_permission_error(command));
	return (0);
}

static char	*check_absolute_path(char *command)
{
	struct stat	st;

	if (access(command, F_OK) == 0)
	{
		if (stat(command, &st) == 0 && S_ISDIR(st.st_mode))
			return (NULL);
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
	}
	return (NULL);
}

char	*find_command_path(char *command, t_minishell *shell)
{
	char		*path_env;
	char		**paths;
	char		*result;
	int			i;

	if (ft_strchr(command, '/'))
		return (check_absolute_path(command));
	path_env = get_env_value("PATH", shell);
	if (!path_env || !*path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	result = search_in_paths(paths, command);
	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
	return (result);
}
