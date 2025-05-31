/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/01 02:32:40 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_directory_error(char *command)
{
	print_error(command, "is a directory");
	return (126);
}

int	handle_permission_error(char *command)
{
	print_error(command, "Permission denied");
	return (126);
}

int	handle_cd_error(char *path)
{
	char	*error_msg;

	error_msg = ft_strjoin(path, ": No such file or directory");
	print_error("cd", error_msg);
	free(error_msg);
	return (1);
}

int	handle_cd_dash(t_minishell *shell)
{
	char	*oldpwd;
	char	*current_pwd;

	oldpwd = get_env_value("OLDPWD", shell);
	if (!oldpwd)
	{
		print_error("cd", "OLDPWD not set");
		return (1);
	}
	current_pwd = get_env_value("PWD", shell);
	if (chdir(oldpwd) != 0)
		return (handle_cd_error(oldpwd));
	ft_putstr_fd(oldpwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	update_pwd_env(current_pwd, shell);
	return (0);
}
