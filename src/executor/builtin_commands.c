/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/06/01 02:33:58 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_echo(char **args)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	while (args[i] && is_valid_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}

int	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (1);
	}
	ft_putstr_fd(cwd, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	free(cwd);
	return (0);
}

void	update_pwd_env(char *old_pwd, t_minishell *shell)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		if (old_pwd)
			set_env_value("OLDPWD", old_pwd, shell);
		set_env_value("PWD", new_pwd, shell);
		free(new_pwd);
	}
}

int	builtin_cd(char **args, t_minishell *shell)
{
	char	*path;
	char	*old_pwd;

	if (args[1] && ft_strncmp(args[1], "-", 1) == 0 && ft_strlen(args[1]) == 1)
		return (handle_cd_dash(shell));
	if (args[1] && args[2])
	{
		print_error("cd", "too many arguments");
		return (1);
	}
	old_pwd = get_env_value("PWD", shell);
	if (!args[1])
		path = get_env_value("HOME", shell);
	else
		path = args[1];
	if (!path)
	{
		print_error("cd", "HOME not set");
		return (1);
	}
	if (chdir(path) != 0)
		return (handle_cd_error(path));
	update_pwd_env(old_pwd, shell);
	return (0);
}

int	builtin_env(char **args, t_minishell *shell)
{
	t_env	*current;

	if (args[1])
	{
		return (1);
	}
	current = shell->env_list;
	while (current)
	{
		if (current->value)
		{
			ft_putstr_fd(current->key, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd(current->value, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		current = current->next;
	}
	return (0);
}
